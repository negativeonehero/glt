#include "gles.h" // The one header to rule them all
#include "cache.h"
#include "state.h"
#include <stdlib.h>

static void* gles_handle = NULL;
static void* egl_handle = NULL;

// Expose handles for glx.c to use
void* get_gles_lib_handle() { return gles_handle; }
void* get_egl_lib_handle() { return egl_handle; }


__attribute__((constructor))
void initialize_translation_layer() {
    fprintf(stderr, "--- OpenGL-to-GLES Translation Layer Initializing ---\n");

    // 1. Load native libraries
    const char* egl_path = getenv("LIBGL_EGL");
    if (!egl_path) egl_path = "libEGL.so";
    egl_handle = dlopen(egl_path, RTLD_LAZY | RTLD_GLOBAL);
    if (!egl_handle) {
        fprintf(stderr, "Layer: Failed to load EGL library from '%s': %s\n", egl_path, dlerror());
        exit(1);
    }
    const char* gles_path = getenv("LIBGL_GLES");
    if (!gles_path) gles_path = "libGLESv2.so"; // Default
    gles_handle = dlopen(gles_path, RTLD_LAZY | RTLD_GLOBAL);
    if (!gles_handle) {
        fprintf(stderr, "Layer: Failed to load GLES library from '%s': %s\n", gles_path, dlerror());
        exit(1);
    }
    fprintf(stderr, "Layer: Native EGL and GLES libraries loaded.\n");

    // 2. Perform initial, best-effort load.
    // EGL functions should load fine.
    if (load_egl_functions(egl_handle) != 0) {
        fprintf(stderr, "Layer: Failed to bind EGL functions. Aborting.\n");
        exit(1);
    }
    shader_cache_init();
    fprintf(stderr, "--- Translation Layer Initialized Successfully (pre-bridge) ---\n");
}

__attribute__((destructor))
void shutdown_translation_layer() {
    fprintf(stderr, "--- Translation Layer Shutting Down ---\n");
    shader_cache_shutdown();
    if (gles_handle) dlclose(gles_handle);
    if (egl_handle) dlclose(egl_handle);
}
