#include "gles.h" // The one header to rule them all
#include "cache.h"
#include <stdlib.h>

static void* gles_handle = NULL;
static void* egl_handle = NULL;

__attribute__((constructor))
void initialize_translation_layer() {
    fprintf(stderr, "--- OpenGL-to-GLES Translation Layer Initializing ---\n");

    // 1. Load native libraries
    egl_handle = dlopen("libEGL.so", RTLD_LAZY | RTLD_GLOBAL);
    if (!egl_handle) {
        fprintf(stderr, "Layer: Failed to load libEGL.so: %s\n", dlerror());
        exit(1); // Or handle error gracefully
    }

    gles_handle = dlopen("libGLESv2.so", RTLD_LAZY | RTLD_GLOBAL);
    if (!gles_handle) {
        fprintf(stderr, "Layer: Failed to load libGLESv2.so: %s\n", dlerror());
        exit(1);
    }
    fprintf(stderr, "Layer: Native EGL and GLES libraries loaded.\n");

    // 2. Load function pointers using the generated loaders
    if (load_egl_functions(egl_handle) != 0) {
        fprintf(stderr, "Layer: Failed to bind EGL functions. Aborting.\n");
        exit(1);
    }

    if (load_gles_functions(gles_handle) != 0) {
        fprintf(stderr, "Layer: Failed to bind GLES functions. Aborting.\n");
        exit(1);
    }
    shader_cache_init();
    fprintf(stderr, "--- Translation Layer Initialized Successfully ---\n");
}

__attribute__((destructor))
void shutdown_translation_layer() {
    fprintf(stderr, "--- Translation Layer Shutting Down ---\n");
    shader_cache_shutdown();
    if (gles_handle) dlclose(gles_handle);
    if (egl_handle) dlclose(egl_handle);
}
