#define _GNU_SOURCE

#include "gles.h" // To get access to the 'egl' struct
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

// We need the real X11 and GLX headers for type definitions
#include <X11/Xlib.h>
#include <GL/glx.h>

// --- Global State for our EGL Bridge ---
// A simple bridge for single-window applications can use global state.
static EGLDisplay g_egl_display = EGL_NO_DISPLAY;
static EGLConfig  g_egl_config = NULL;
static EGLContext g_egl_context = EGL_NO_CONTEXT;
static EGLSurface g_egl_surface = EGL_NO_SURFACE;
static GLXDrawable g_current_drawable = 0;

// Handle to our own library, to prevent symbol leakage from libGL.so
static void* g_self_handle = NULL;


// --- Helper Functions ---

static void ensure_egl_display(Display* dpy) {
    if (g_egl_display == EGL_NO_DISPLAY) {
        printf("[Bridge] Initializing EGL display...\n");
        // We get the EGL display from the native X11 display
        g_egl_display = egl.eglGetDisplay((EGLNativeDisplayType)dpy);
        if (g_egl_display == EGL_NO_DISPLAY) {
            fprintf(stderr, "[Bridge] eglGetDisplay failed!\n");
            return;
        }

        EGLint major, minor;
        if (!egl.eglInitialize(g_egl_display, &major, &minor)) {
            fprintf(stderr, "[Bridge] eglInitialize failed!\n");
            return;
        }
        printf("[Bridge] EGL Initialized: Version %d.%d\n", major, minor);
    }
}

// --- Intercepted GLX Functions ---

// The application asks for a list of suitable framebuffer configurations.
GLXFBConfig* glXChooseFBConfig(Display* dpy, int screen, const int* attrib_list, int* nelements) {
    printf("[Bridge] Intercepted glXChooseFBConfig\n");
    ensure_egl_display(dpy);

    // A basic attribute list that requests a GLES2-compatible, window-renderable config
    const EGLint egl_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };

    EGLConfig* configs = NULL;
    EGLint num_config;

    // Ask EGL for a list of matching configs
    if (!egl.eglChooseConfig(g_egl_display, egl_attribs, NULL, 0, &num_config) || num_config == 0) {
        fprintf(stderr, "[Bridge] eglChooseConfig failed to find any configs.\n");
        *nelements = 0;
        return NULL;
    }

    configs = (EGLConfig*)malloc(num_config * sizeof(EGLConfig));
    if (!egl.eglChooseConfig(g_egl_display, egl_attribs, configs, num_config, &num_config)) {
        fprintf(stderr, "[Bridge] eglChooseConfig (second call) failed.\n");
        free(configs);
        *nelements = 0;
        return NULL;
    }

    printf("[Bridge] Found %d matching EGL configs.\n", num_config);
    *nelements = num_config;

    // We return the array of EGLConfig pointers, pretending they are GLXFBConfig pointers.
    // This is the core of our simple mapping strategy.
    return (GLXFBConfig*)configs;
}

// The application needs an XVisualInfo to create its window.
XVisualInfo* glXGetVisualFromFBConfig(Display* dpy, GLXFBConfig config) {
    printf("[Bridge] Intercepted glXGetVisualFromFBConfig\n");
    EGLint visual_id;

    // The 'config' parameter is really an EGLConfig from our function above.
    if (!egl.eglGetConfigAttrib(g_egl_display, (EGLConfig)config, EGL_NATIVE_VISUAL_ID, &visual_id)) {
        fprintf(stderr, "[Bridge] eglGetConfigAttrib for EGL_NATIVE_VISUAL_ID failed.\n");
        return NULL;
    }

    XVisualInfo visual_template;
    visual_template.visualid = visual_id;
    int nitems;
    return XGetVisualInfo(dpy, VisualIDMask, &visual_template, &nitems);
}

GLXContext glXCreateContextAttribsARB(Display* dpy,
                                      GLXFBConfig config,
                                      GLXContext share_context,
                                      Bool direct,
                                      const int* attrib_list) {
    printf("[Bridge] Intercepted glXCreateContextAttribsARB\n");
    (void)dpy; (void)direct; // Unused parameters

    ensure_egl_display(dpy);
    EGLConfig egl_config = (EGLConfig)config;

    // If the application didn't provide a config, we must choose a default one.
    if (egl_config == NULL) {
        printf("[Bridge] No FBConfig provided to glXCreateContextAttribsARB, choosing a default.\n");
        int nelements;
        // Use our existing chooser. Note: This leaks memory in our simple implementation.
        GLXFBConfig* configs = glXChooseFBConfig(dpy, 0, NULL, &nelements);
        if (nelements > 0 && configs) {
            egl_config = (EGLConfig)configs[0];
            // ToDo: The 'configs' array is leaked here. A real implementation should manage this memory.
        } else {
            fprintf(stderr, "[Bridge] Could not find a default EGLConfig for context creation.\n");
            return NULL;
        }
    }

    g_egl_config = egl_config;

    // EGL attribute list we will build.
    EGLint egl_attribs[30];
    int i = 0;
    int major_ver = 2; // Default to GLES 2.0
    int minor_ver = 0;

    if (attrib_list) {
        int p = 0;
        while (attrib_list[p] != None) {
            switch (attrib_list[p]) {
                case GLX_CONTEXT_MAJOR_VERSION_ARB:
                    major_ver = attrib_list[p+1];
                    printf("[Bridge] -> Requested GL Major Version: %d\n", major_ver);
                    break;
                case GLX_CONTEXT_MINOR_VERSION_ARB:
                    minor_ver = attrib_list[p+1];
                    printf("[Bridge] -> Requested GL Minor Version: %d\n", minor_ver);
                    break;
                case GLX_CONTEXT_PROFILE_MASK_ARB:
                    if (attrib_list[p+1] & GLX_CONTEXT_CORE_PROFILE_BIT_ARB) {
                        printf("[Bridge] -> Requested Core Profile (compatible with GLES)\n");
                    }
                    if (attrib_list[p+1] & GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB) {
                        fprintf(stderr, "[Bridge] WARNING: Application requested a Compatibility Profile, which GLES does not support. Proceeding as Core.\n");
                    }
                    break;
                case GLX_CONTEXT_FLAGS_ARB:
                    printf("[Bridge] -> Context Flags: 0x%x\n", attrib_list[p+1]);
                    break;
                default:
                    fprintf(stderr, "[Bridge] -> Unknown GLX context attribute: 0x%x\n", attrib_list[p]);
                    break;
            }
            p += 2;
        }
    }

    egl_attribs[i++] = EGL_CONTEXT_CLIENT_VERSION;
    egl_attribs[i++] = major_ver;
    egl_attribs[i++] = EGL_NONE;

    printf("[Bridge] Creating EGL context for GLES %d.%d using config %p\n", major_ver, minor_ver, g_egl_config);
    g_egl_context = egl.eglCreateContext(g_egl_display, g_egl_config, (EGLContext)share_context, egl_attribs);

    if (g_egl_context == EGL_NO_CONTEXT) {
        fprintf(stderr, "[Bridge] eglCreateContext failed! EGL error: 0x%x\n", egl.eglGetError());
        return NULL;
    }

    printf("[Bridge] EGL context created successfully via ARB attributes.\n");
    return (GLXContext)g_egl_context;
}

GLXContext glXCreateNewContext(Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct) {
    printf("[Bridge] Intercepted glXCreateNewContext\n");
    (void)render_type; // Unused, GLES is always RGBA
    return glXCreateContextAttribsARB(dpy, config, share_list, direct, NULL);
}

GLXContext glXCreateContext(Display* dpy, XVisualInfo* vis, GLXContext share_list, Bool direct) {
    printf("[Bridge] Intercepted legacy glXCreateContext. Forwarding to ARB version with default attributes.\n");
    // In a real implementation, you might need to find an EGLConfig that matches the XVisualInfo.
    // For this simple bridge, we assume a compatible config was already found and stored in g_egl_config.
    if (g_egl_config == NULL) {
        fprintf(stderr, "[Bridge] Error: legacy glXCreateContext called before a config was chosen.\n");
    }
    return glXCreateContextAttribsARB(dpy, (GLXFBConfig)g_egl_config, share_list, direct, NULL);
}

void glXDestroyContext(Display *dpy, GLXContext ctx) {
    printf("[Bridge] Intercepted glXDestroyContext\n");
    if (g_egl_display != EGL_NO_DISPLAY && (EGLContext)ctx != EGL_NO_CONTEXT) {
        egl.eglDestroyContext(g_egl_display, (EGLContext)ctx);
    }
}

Bool glXMakeCurrent(Display* dpy, GLXDrawable drawable, GLXContext ctx) {
    printf("[Bridge] Intercepted legacy glXMakeCurrent, forwarding to glXMakeContextCurrent\n");
    return glXMakeContextCurrent(dpy, drawable, drawable, ctx);
}

Bool glXMakeContextCurrent(Display* dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
    printf("[Bridge] Intercepted glXMakeContextCurrent for draw=0x%lx, read=0x%lx\n", draw, read);
    (void)dpy; // dpy is not used with EGL

    EGLContext egl_ctx = (EGLContext)ctx;
    EGLSurface egl_draw_surface = EGL_NO_SURFACE;
    EGLSurface egl_read_surface = EGL_NO_SURFACE;

    // This is a simplified implementation. A full implementation would need to
    // manage separate surfaces for read and draw if they are different.
    // For now, we'll assume they are the same, which covers 99% of cases.
    // If the drawable is changing, we need to create a new EGLSurface.
    if (g_current_drawable != draw) {
        if (g_egl_surface != EGL_NO_SURFACE) {
            // Unbind the old context before destroying the surface
            egl.eglMakeCurrent(g_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            egl.eglDestroySurface(g_egl_display, g_egl_surface);
        }

        if (draw != 0) {
            printf("[Bridge] Creating new EGLSurface for drawable.\n");
            // The config should have been set during context creation
            if (g_egl_config == NULL) {
                fprintf(stderr, "[Bridge] Error: No EGLConfig available to create a surface.\n");
                return False;
            }
            egl_draw_surface = egl.eglCreateWindowSurface(g_egl_display, g_egl_config, (EGLNativeWindowType)draw, NULL);
            if (egl_draw_surface == EGL_NO_SURFACE) {
                fprintf(stderr, "[Bridge] eglCreateWindowSurface failed! EGL error: 0x%x\n", egl.eglGetError());
                return False;
            }
            // In this simple case, read surface is the same as draw surface
            egl_read_surface = egl_draw_surface;
            g_egl_surface = egl_draw_surface; // Store the new surface globally
        } else {
            // Unbinding from all surfaces
            g_egl_surface = EGL_NO_SURFACE;
        }
        g_current_drawable = draw;
    } else {
        // We are binding to the same drawable as before, so reuse the existing surface
        egl_draw_surface = g_egl_surface;
        egl_read_surface = g_egl_surface;
    }

    // Finally, call the real eglMakeCurrent with the mapped surfaces and context
    return egl.eglMakeCurrent(g_egl_display, egl_draw_surface, egl_read_surface, egl_ctx);
}

// The application swaps the front and back buffers.
void glXSwapBuffers(Display* dpy, GLXDrawable drawable) {
    // We don't need to log this one, it's called every frame.
    (void)dpy; (void)drawable;
    egl.eglSwapBuffers(g_egl_display, g_egl_surface);
}

const char* glXQueryExtensionsString(Display* dpy, int screen) {
    printf("[Bridge] Intercepted glXQueryExtensionsString\n");
    (void)screen; // Unused parameter

    ensure_egl_display(dpy);

    if (g_egl_display == EGL_NO_DISPLAY) {
        fprintf(stderr, "[Bridge] EGL display not available for extension query.\n");
        return ""; // Return empty string on failure
    }

    // The most direct translation is to return EGL's own extension string.
    const char* exts = egl.eglQueryString(g_egl_display, EGL_EXTENSIONS);
    if (!exts) {
        return "";
    }
    
    printf("[Bridge] Reporting EGL extensions as GLX extensions.\n");
    // A more complete implementation might append its own emulated extensions,
    // like GLX_ARB_create_context.
    return exts;
}

// The application wants to know if a specific extension is supported.
Bool glXQueryExtension(Display* dpy, int* error_base, int* event_base) {
    printf("[Bridge] Intercepted glXQueryExtension\n");
    (void)dpy; // Unused

    // We can just report that the GLX extension is available.
    // The real checks happen when the app queries for specific extension strings.
    if (error_base) *error_base = 0;
    if (event_base) *event_base = 0;

    // Returning True here is generally safe. It tells the application
    // that the GLX mechanism itself is present. The app will then use
    // glXQueryExtensionsString or glXGetProcAddress to check for specifics.
    return True;
}

Bool glXIsDirect(Display *dpy, GLXContext ctx) {
    printf("[Bridge] Intercepted glXIsDirect\n");
    (void)dpy; // Unused parameter
    (void)ctx; // Unused parameter

    // In our EGL-based translation layer, all contexts are conceptually "direct".
    // There is no indirect rendering path via the X server.
    return True;
}

// Forward declaration for glXGetProcAddressARB
void (*glXGetProcAddress(const GLubyte *procname))();

// Some apps use the ARB version, so we just make it an alias.
void (*glXGetProcAddressARB(const GLubyte *procname))() {
    return glXGetProcAddress(procname);
}

void (*glXGetProcAddress(const GLubyte *procname))() {
    const char* name = (const char*)procname;
    printf("[Bridge] Intercepted glXGetProcAddress for: %s\n", name);

    // --- Step 1: Check for our own implemented GLX functions first. ---
    // This is the most important part: we explicitly return pointers to our
    // own bridge functions, preventing any system versions from being used.
    if (strcmp(name, "glXChooseFBConfig") == 0) return (void (*)())glXChooseFBConfig;
    if (strcmp(name, "glXGetVisualFromFBConfig") == 0) return (void (*)())glXGetVisualFromFBConfig;
    if (strcmp(name, "glXCreateContext") == 0) return (void (*)())glXCreateContext;
    if (strcmp(name, "glXCreateContextAttribsARB") == 0) return (void (*)())glXCreateContextAttribsARB;
    if (strcmp(name, "glXDestroyContext") == 0) return (void (*)())glXDestroyContext;
    if (strcmp(name, "glXMakeCurrent") == 0) return (void (*)())glXMakeCurrent;
    if (strcmp(name, "glXMakeContextCurrent") == 0) return (void (*)())glXMakeContextCurrent;
    if (strcmp(name, "glXSwapBuffers") == 0) return (void (*)())glXSwapBuffers;
    if (strcmp(name, "glXQueryExtensionsString") == 0) return (void (*)())glXQueryExtensionsString;
    if (strcmp(name, "glXQueryExtension") == 0) return (void (*)())glXQueryExtension;
    if (strcmp(name, "glXCreateNewContext") == 0) return (void (*)())glXCreateNewContext;
    if (strcmp(name, "glXGetProcAddress") == 0) return (void (*)())glXGetProcAddress;
    // Fix for potential recursion: glXGetProcAddressARB should point to the main function.
    if (strcmp(name, "glXGetProcAddressARB") == 0) return (void (*)())glXGetProcAddress;
    if (strcmp(name, "glXIsDirect") == 0) return (void (*)())glXIsDirect;
    // Add any other GLX functions you implement here...

    // --- Step 2: Check for core GL functions we have stubbed in THIS library. ---
    // This is the critical fix. We use a handle to our own library to ensure
    // we don't accidentally load a real OpenGL function from the system's libGL.
    if (g_self_handle == NULL) {
        // One-time initialization. Get a handle to our own .so file.
        // We use dladdr to find the path of our library from a known function symbol.
        Dl_info info;
        if (dladdr((void*)glXGetProcAddress, &info)) {
            // Now open our own library to get a handle for dlsym.
            // RTLD_LAZY: resolve symbols as code is executed.
            // RTLD_NOLOAD: don't load the library if it's not already loaded (it will be).
            g_self_handle = dlopen(info.dli_fname, RTLD_LAZY | RTLD_NOLOAD);
        }
        if (g_self_handle == NULL) {
            fprintf(stderr, "[Bridge] FATAL: Could not get a handle to our own library. %s\n", dlerror());
            // We cannot proceed safely.
        }
    }

    void* func = NULL;
    if (g_self_handle) {
        // Use our own handle to find symbols. This will find functions in your
        // opengl_stubs.c but will NOT find them in the system libGL.so.1.
        func = dlsym(g_self_handle, name);
    }
    
    if (func) {
        return (void (*)())func;
    }

    // --- Step 3: If it's not one of our functions, ask the EGL/GLES driver. ---
    // This is the safe way to get pointers to GLES extension functions that we
    // haven't explicitly stubbed (e.g., glDrawArraysInstancedANGLE).
    if (egl.eglGetProcAddress) {
        func = (void*)egl.eglGetProcAddress(name);
        if (func) {
            return (void (*)())func;
        }
    }

    // --- Step 4: If we found nothing, return NULL. ---
    // This is critical. We are telling the application that we do not support
    // this function, preventing the "leak" of a real GLX function pointer.
    fprintf(stderr, "[Bridge] glXGetProcAddress: Returning NULL for unsupported function '%s'\n", name);
    return NULL;
}
