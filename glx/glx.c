#define _GNU_SOURCE
#include "gles.h" // Your main generated header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dlfcn.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

// --- Forward declarations ---
void* get_gles_lib_handle();
void* get_egl_lib_handle();

// --- Global State for the Bridge ---
static bool g_bridge_initialized = false;
static EGLDisplay g_egl_display = EGL_NO_DISPLAY;
static EGLConfig  g_egl_config = NULL;
static EGLSurface g_egl_surface = EGL_NO_SURFACE;
static EGLContext g_egl_context = EGL_NO_CONTEXT;
static GLXDrawable g_current_drawable = 0;
static void* g_self_handle = NULL;

// --- State for the Pbuffer-based Shim ---
static void* g_pixel_buffer = NULL; // CPU-side buffer for glReadPixels
static int g_window_width = 0;
static int g_window_height = 0;
struct gles_version_t gles_version;

typedef enum {
    SHIM_MODE_UNINITIALIZED, // No surface has been created yet
    SHIM_MODE_NATIVE_X11,    // Using a native EGL window surface (fast path)
    SHIM_MODE_PBUFFER_COPY   // Using a Pbuffer with glReadPixels (fallback path)
} ShimMode;

static ShimMode g_shim_mode = SHIM_MODE_UNINITIALIZED;

// =================================================================================================
//                                     INITIALIZATION LOGIC
// =================================================================================================

static void ensure_bridge_initialized() {
    if (g_bridge_initialized) return;
    fprintf(stderr, "[Bridge] First GLX call intercepted. Performing Pbuffer initialization...\n");
    void* egl_handle = get_egl_lib_handle();
    if (!egl_handle) { fprintf(stderr, "[Bridge] FATAL: EGL library handle is NULL.\n"); exit(1); }
    if (!egl.eglGetProcAddress) {
        egl.eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC)dlsym(egl_handle, "eglGetProcAddress");
        if (!egl.eglGetProcAddress) { fprintf(stderr, "[Bridge] FATAL: Could not dlsym eglGetProcAddress.\n"); exit(1); }
    }
    EGLDisplay temp_dpy = egl.eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (temp_dpy == EGL_NO_DISPLAY) { fprintf(stderr, "[Bridge] Pbuffer init failed: eglGetDisplay returned EGL_NO_DISPLAY.\n"); exit(1); }
    if (egl.eglInitialize(temp_dpy, NULL, NULL) != EGL_TRUE) { fprintf(stderr, "[Bridge] Pbuffer init failed: eglInitialize failed (0x%x).\n", egl.eglGetError()); exit(1); }
    EGLint config_attribs[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
    EGLConfig config; EGLint num_config;
    if (egl.eglChooseConfig(temp_dpy, config_attribs, &config, 1, &num_config) != EGL_TRUE || num_config == 0) { fprintf(stderr, "[Bridge] Pbuffer init failed: eglChooseConfig failed (0x%x).\n", egl.eglGetError()); exit(1); }
    EGLint ctx_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext temp_ctx = egl.eglCreateContext(temp_dpy, config, EGL_NO_CONTEXT, ctx_attribs);
    if (temp_ctx == EGL_NO_CONTEXT) { fprintf(stderr, "[Bridge] Pbuffer init failed: eglCreateContext failed (0x%x).\n", egl.eglGetError()); exit(1); }
    EGLint pbuf_attribs[] = { EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE };
    EGLSurface temp_surf = egl.eglCreatePbufferSurface(temp_dpy, config, pbuf_attribs);
    if (temp_surf == EGL_NO_SURFACE) { fprintf(stderr, "[Bridge] Pbuffer init failed: eglCreatePbufferSurface failed (0x%x).\n", egl.eglGetError()); exit(1); }
    if (egl.eglMakeCurrent(temp_dpy, temp_surf, temp_surf, temp_ctx) != EGL_TRUE) { fprintf(stderr, "[Bridge] Pbuffer init failed: eglMakeCurrent failed (0x%x).\n", egl.eglGetError()); exit(1); }
    fprintf(stderr, "[Bridge] Temporary context active. Re-loading GLES function pointers...\n");
    if (load_gles_functions(get_gles_lib_handle()) != 0) { fprintf(stderr, "[Bridge] FATAL: Failed to re-bind GLES functions.\n"); exit(1); }
    fprintf(stderr, "[Bridge] GLES functions re-loaded successfully.\n");
    egl.eglMakeCurrent(temp_dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    egl.eglDestroySurface(temp_dpy, temp_surf);
    egl.eglDestroyContext(temp_dpy, temp_ctx);
    egl.eglTerminate(temp_dpy);
    g_bridge_initialized = true;
    fprintf(stderr, "[Bridge] Pbuffer initialization complete.\n");
}

static void ensure_egl_display(Display* dpy) {
    if (g_egl_display != EGL_NO_DISPLAY) return;

    printf("[Bridge] Initializing EGL display for application...\n");

    g_egl_display = egl.eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (g_egl_display == EGL_NO_DISPLAY) {
        fprintf(stderr, "[Bridge] FATAL: eglGetDisplay(EGL_DEFAULT_DISPLAY) failed.\n");
        exit(1);
    }

    EGLint major, minor;
    if (!egl.eglInitialize(g_egl_display, &major, &minor)) {
        fprintf(stderr, "[Bridge] FATAL: eglInitialize failed for the application display (0x%x).\n", egl.eglGetError());
        exit(1);
    }
    printf("[Bridge] EGL Initialized for application: Version %d.%d\n", major, minor);
}

// =================================================================================================
//                                  INTERCEPTED GLX FUNCTIONS
// =================================================================================================

GLXFBConfig* glXChooseFBConfig(Display* dpy, int screen, const int* attrib_list, int* nelements) {
    ensure_bridge_initialized();
    printf("[Bridge] Intercepted glXChooseFBConfig\n");
    ensure_egl_display(dpy);

    const EGLint egl_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24, EGL_STENCIL_SIZE, 8, EGL_NONE
    };

    EGLint num_config;
    if (!egl.eglChooseConfig(g_egl_display, egl_attribs, NULL, 0, &num_config) || num_config == 0) {
        fprintf(stderr, "[Bridge] eglChooseConfig failed to find any suitable configs.\n");
        *nelements = 0; return NULL;
    }
    EGLConfig* configs = (EGLConfig*)malloc(num_config * sizeof(EGLConfig));
    if (!egl.eglChooseConfig(g_egl_display, egl_attribs, configs, num_config, &num_config)) {
        free(configs); *nelements = 0; return NULL;
    }
    printf("[Bridge] Found %d matching EGL configs.\n", num_config);
    *nelements = num_config;
    return (GLXFBConfig*)configs;
}

XVisualInfo* glXGetVisualFromFBConfig(Display* dpy, GLXFBConfig config) {
    ensure_bridge_initialized();
    printf("[Bridge] Intercepted glXGetVisualFromFBConfig\n");
    printf("[Bridge] Providing screen's default visual to prevent XCreateWindow BadMatch.\n");

    int screen = DefaultScreen(dpy);
    Visual* default_visual = DefaultVisual(dpy, screen);
    XVisualInfo visual_template = { .visualid = XVisualIDFromVisual(default_visual) };
    int nitems = 0;
    XVisualInfo* vi = XGetVisualInfo(dpy, VisualIDMask, &visual_template, &nitems);
    if (vi && nitems > 0) {
        printf("[Bridge] Successfully found XVisualInfo for the default screen visual (ID: %ld, Depth: %d).\n", vi->visualid, vi->depth);
        return vi;
    }
    fprintf(stderr, "[Bridge] FATAL: Could not find XVisualInfo for the default visual.\n");
    return NULL;
}

GLXContext glXCreateContextAttribsARB(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int* attrib_list) {
    ensure_bridge_initialized();
    printf("[Bridge] Intercepted glXCreateContextAttribsARB\n");
    g_egl_config = (EGLConfig)config;
    // Always assume GLES 3.0+ support.
    EGLint egl_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 2, EGL_NONE };
    printf("[Bridge] Creating EGL context for GLES 3.2\n");
    while(g_egl_context == EGL_NO_CONTEXT && egl_attribs[3] > 0) {
        g_egl_context = egl.eglCreateContext(g_egl_display, g_egl_config, (EGLContext)share_context, egl_attribs);
        if (g_egl_context == EGL_NO_CONTEXT) {
            egl_attribs[3] -= 1;
            fprintf(stderr, "[Bridge] eglCreateContext failed! Trying GLES %d.%d\n", 3, egl_attribs[3]);
        }
    }
    if (g_egl_context == EGL_NO_CONTEXT) { fprintf(stderr, "[Bridge] eglCreateContext failed! EGL error: 0x%x\n", egl.eglGetError()); }
    gles_version.major = 3; gles_version.minor = egl_attribs[3];
    return (GLXContext)g_egl_context;
}

Bool glXMakeContextCurrent(Display* dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
    ensure_bridge_initialized();
    if (g_current_drawable != draw) {
        if (g_egl_surface != EGL_NO_SURFACE) {
            egl.eglDestroySurface(g_egl_display, g_egl_surface);
            g_egl_surface = EGL_NO_SURFACE;
        }
        if (g_pixel_buffer) {
            free(g_pixel_buffer);
            g_pixel_buffer = NULL;
        }
        g_shim_mode = SHIM_MODE_UNINITIALIZED;

        if (draw != 0) {
            printf("[Bridge] Attempting to create a native EGL window surface (fast path)...\n");
            g_egl_surface = egl.eglCreateWindowSurface(g_egl_display, g_egl_config, (EGLNativeWindowType)draw, NULL);

            if (g_egl_surface != EGL_NO_SURFACE) {
                printf("[Bridge] Success! Using native EGL window surface.\n");
                g_shim_mode = SHIM_MODE_NATIVE_X11;
            } else {
                fprintf(stderr, "[Bridge] Native window surface creation failed (EGL error: 0x%x).\n", egl.eglGetError());
                fprintf(stderr, "[Bridge] Falling back to Pbuffer copy method (slower path).\n");

                Window root; int x, y; unsigned int w, h, border, depth;
                XGetGeometry(dpy, draw, &root, &x, &y, &w, &h, &border, &depth);
                g_window_width = w;
                g_window_height = h;
                g_pixel_buffer = malloc(w * h * 4);

                const EGLint pbuf_attribs[] = { EGL_WIDTH, w, EGL_HEIGHT, h, EGL_NONE };
                g_egl_surface = egl.eglCreatePbufferSurface(g_egl_display, g_egl_config, pbuf_attribs);

                if (g_egl_surface != EGL_NO_SURFACE) {
                    printf("[Bridge] Fallback Pbuffer surface created successfully.\n");
                    g_shim_mode = SHIM_MODE_PBUFFER_COPY;
                } else {
                    fprintf(stderr, "[Bridge] FATAL: Fallback Pbuffer surface creation also failed (EGL error: 0x%x).\n", egl.eglGetError());
                    free(g_pixel_buffer);
                    g_pixel_buffer = NULL;
                    return False;
                }
            }
        }
        g_current_drawable = draw;
    }
    return egl.eglMakeCurrent(g_egl_display, g_egl_surface, g_egl_surface, (EGLContext)ctx);
}

void glXSwapBuffers(Display* dpy, GLXDrawable drawable) {
    ensure_bridge_initialized();

    switch (g_shim_mode) {
        case SHIM_MODE_NATIVE_X11:
            // FAST PATH: The driver handles the swap directly.
            egl.eglSwapBuffers(g_egl_display, g_egl_surface);
            break;

        case SHIM_MODE_PBUFFER_COPY:
            // FALLBACK PATH: We must manually copy pixels from our off-screen Pbuffer.
            if (!g_pixel_buffer || !gles.core.glReadPixels) return;

            gles.core.glReadPixels(0, 0, g_window_width, g_window_height, GL_RGBA, GL_UNSIGNED_BYTE, g_pixel_buffer);

            XImage* image = XCreateImage(dpy, DefaultVisual(dpy, DefaultScreen(dpy)),
                                         DefaultDepth(dpy, DefaultScreen(dpy)), ZPixmap, 0,
                                         (char*)g_pixel_buffer, g_window_width, g_window_height, 32, 0);
            if (!image) { fprintf(stderr, "[Bridge] XCreateImage failed!\n"); return; }
            GC gc = XCreateGC(dpy, drawable, 0, NULL);
            XPutImage(dpy, drawable, gc, image, 0, 0, 0, 0, g_window_width, g_window_height);
            XFreeGC(dpy, gc);
            XFlush(dpy);

            image->data = NULL;
            XDestroyImage(image);
            break;
        case SHIM_MODE_UNINITIALIZED:
            // No surface is current, do nothing.
            break;
    }
}

// --- Aliases, Legacy Functions, and Info Functions ---

const char* glXQueryExtensionsString(Display* dpy, int screen) { return ""; }
Bool glXQueryExtension(Display* dpy, int* error_base, int* event_base) { return True; }
GLXContext glXCreateNewContext(Display *dpy, GLXFBConfig config, int r_type, GLXContext share, Bool direct) { return glXCreateContextAttribsARB(dpy, config, share, direct, NULL); }
GLXContext glXCreateContext(Display* dpy, XVisualInfo* vis, GLXContext share, Bool direct) { return glXCreateContextAttribsARB(dpy, (GLXFBConfig)g_egl_config, share, direct, NULL); }
void glXDestroyContext(Display *dpy, GLXContext ctx) { if (g_egl_display != EGL_NO_DISPLAY && (EGLContext)ctx != EGL_NO_CONTEXT) egl.eglDestroyContext(g_egl_display, (EGLContext)ctx); }
Bool glXMakeCurrent(Display* dpy, GLXDrawable drawable, GLXContext ctx) { return glXMakeContextCurrent(dpy, drawable, drawable, ctx); }
Bool glXIsDirect(Display *dpy, GLXContext ctx) { return True; }


// =================================================================================================
//                                  SYMBOL INTERCEPTION (CRITICAL)
// =================================================================================================

void (*glXGetProcAddress(const GLubyte *procname))();
void (*glXGetProcAddressARB(const GLubyte *procname))() { return glXGetProcAddress(procname); }

void (*glXGetProcAddress(const GLubyte *procname))() {
    const char* name = (const char*)procname;
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
    if (strcmp(name, "glXGetProcAddressARB") == 0) return (void (*)())glXGetProcAddress;
    if (strcmp(name, "glXIsDirect") == 0) return (void (*)())glXIsDirect;
    
    if (g_self_handle == NULL) {
        Dl_info info;
        if (dladdr((void*)glXGetProcAddress, &info)) {
            g_self_handle = dlopen(info.dli_fname, RTLD_LAZY | RTLD_NOLOAD);
        }
    }
    void* func = NULL;
    if (g_self_handle) func = dlsym(g_self_handle, name);
    if (func) return (void (*)())func;

    ensure_bridge_initialized();
    if (egl.eglGetProcAddress) {
        func = (void*)egl.eglGetProcAddress(name);
        if (func) return (void (*)())func;
    }
    fprintf(stderr, "[Bridge] glXGetProcAddress: Returning NULL for unsupported function '%s'\n", name);
    return NULL;
}
