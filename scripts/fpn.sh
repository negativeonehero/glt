#!/bin/bash

set -e

# --- Configuration ---
GLES_LIB_PATH="$1"
EGL_LIB_PATH="$2"
OUTPUT_HEADER="gl_procs.h"  # Renamed to be more generic
OUTPUT_IMPL="gl_loader.c"   # Renamed to be more generic

# --- Validation ---
if [ -z "$GLES_LIB_PATH" ] || [ -z "$EGL_LIB_PATH" ]; then
    echo "Usage: $0 /path/to/libGLES.so /path/to/libEGL.so"
    echo "Example: ./generate_loader.sh /usr/lib/libGLESv2.so.2 /usr/lib/libEGL.so.1"
    exit 1
fi

for lib in "$GLES_LIB_PATH" "$EGL_LIB_PATH"; do
    if [ ! -f "$lib" ]; then
        echo "Error: Library not found at '$lib'"
        exit 1
    fi
done

echo "--- Generating GLES and EGL loaders ---"
echo "GLES Library: $GLES_LIB_PATH"
echo "EGL Library:  $EGL_LIB_PATH"

# --- GLES Function Extraction ---
echo "Extracting and categorizing GLES functions..."
GLES_VENDOR_TAGS_PATTERN="(EXT|OES|KHR|NV|AMD|INTEL|IMG|QCOM|ANGLE|ARM|APPLE|OVR)$"
declare -a gles_core_funcs
declare -a gles_ext_funcs

while IFS= read -r func; do
    if [ -z "$func" ]; then continue; fi
    if echo "$func" | grep -qE "$GLES_VENDOR_TAGS_PATTERN"; then
        gles_ext_funcs+=("$func")
    else
        gles_core_funcs+=("$func")
    fi
done < <(readelf -s --wide "$GLES_LIB_PATH" | awk '$4 == "FUNC" && $NF ~ /^gl/ {print $NF}' | sort -u)

echo "Found ${#gles_core_funcs[@]} GLES core functions and ${#gles_ext_funcs[@]} GLES extension functions."

# --- EGL Function Extraction ---
echo "Extracting EGL functions..."
declare -a egl_funcs

while IFS= read -r func; do
    if [ -z "$func" ]; then continue; fi
    egl_funcs+=("$func")
done < <(readelf -s --wide "$EGL_LIB_PATH" | awk '$4 == "FUNC" && $NF ~ /^egl/ {print $NF}' | sort -u)

echo "Found ${#egl_funcs[@]} EGL functions."


# -----------------------------------------------------------------------------
# 1. Generate the Header File (gl_procs.h)
# -----------------------------------------------------------------------------
echo "Generating header file: $OUTPUT_HEADER"
cat > "$OUTPUT_HEADER" <<EOF
#ifndef GL_PROCS_H
#define GL_PROCS_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl32.h>
#include <dlfcn.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- GLES Struct Definitions ---
struct gles_core_procs {
$(for func in "${gles_core_funcs[@]}"; do
    pfn_type="PFN${func^^}PROC"
    echo "    $pfn_type $func;"
done)
};

struct gles_ext_procs {
$(for func in "${gles_ext_funcs[@]}"; do
    pfn_type="PFN${func^^}PROC"
    echo "    $pfn_type $func;"
done)
};

struct gles_t {
    struct gles_core_procs core;
    struct gles_ext_procs ext;
};

// --- EGL Struct Definition ---
struct egl_t {
$(for func in "${egl_funcs[@]}"; do
    pfn_type="PFN${func^^}PROC"
    echo "    $pfn_type $func;"
done)
};

// --- Global Declarations ---
extern struct gles_t gles;
extern struct egl_t egl;

// --- Loader Macro Definitions ---
#ifdef __cplusplus
// C++ versions using decltype
#define LOAD_CORE_PROC(func) \\
    do { \\
        gles.core.func = (decltype(gles.core.func))dlsym(handle, #func); \\
        if (!gles.core.func) { fprintf(stderr, "Loader: Failed to load GLES core '%s'\\n", #func); } \\
    } while (0)

#define LOAD_EXT_PROC(func) \\
    do { gles.ext.func = (decltype(gles.ext.func))dlsym(handle, #func); } while (0)

#define LOAD_EGL_PROC(func) \\
    do { \\
        egl.func = (decltype(egl.func))dlsym(handle, #func); \\
        if (!egl.func) { fprintf(stderr, "Loader: Failed to load EGL function '%s'\\n", #func); } \\
    } while (0)

#else
// C versions requiring explicit types
#define LOAD_CORE_PROC(func, type) \\
    do { \\
        gles.core.func = (type)dlsym(handle, #func); \\
        if (!gles.core.func) { fprintf(stderr, "Loader: Failed to load GLES core '%s'\\n", #func); } \\
    } while (0)

#define LOAD_EXT_PROC(func, type) \\
    do { gles.ext.func = (type)dlsym(handle, #func); } while (0)

#define LOAD_EGL_PROC(func, type) \\
    do { \\
        egl.func = (type)dlsym(handle, #func); \\
        if (!egl.func) { fprintf(stderr, "Loader: Failed to load EGL function '%s'\\n", #func); } \\
    } while (0)
#endif

// --- Public API ---
int load_gles_functions(void* handle);
int load_egl_functions(void* handle);

#ifdef __cplusplus
}
#endif

#endif // GL_PROCS_H
EOF


# -----------------------------------------------------------------------------
# 2. Generate the Implementation File (gl_loader.c)
# -----------------------------------------------------------------------------
echo "Generating implementation file: $OUTPUT_IMPL"
cat > "$OUTPUT_IMPL" <<EOF
#include "gl_procs.h"

// Define global instances of our function tables.
struct gles_t gles;
struct egl_t egl;

int load_gles_functions(void* handle) {
    if (!handle) {
        fprintf(stderr, "Loader: Invalid GLES library handle provided.\\n");
        return -1;
    }

    // Load Core GLES Functions
$(for func in "${gles_core_funcs[@]}"; do
    pfn_type="PFN${func^^}PROC"
    echo "    LOAD_CORE_PROC($func, $pfn_type);"
done)

    // Load Extension GLES Functions
$(for func in "${gles_ext_funcs[@]}"; do
    pfn_type="PFN${func^^}PROC"
    echo "    LOAD_EXT_PROC($func, $pfn_type);"
done)

    // Final check for critical GLES functions
    if (!gles.core.glClear || !gles.core.glDrawArrays || !gles.core.glGetString) {
        fprintf(stderr, "Loader: CRITICAL FAILURE. Essential GLES functions could not be loaded.\\n");
        return -1;
    }
    return 0;
}

int load_egl_functions(void* handle) {
    if (!handle) {
        fprintf(stderr, "Loader: Invalid EGL library handle provided.\\n");
        return -1;
    }

    // Load EGL Functions
$(for func in "${egl_funcs[@]}"; do
    pfn_type="PFN${func^^}PROC"
    echo "    LOAD_EGL_PROC($func, $pfn_type);"
done)

    // Final check for critical EGL functions
    if (!egl.eglGetDisplay || !egl.eglInitialize || !egl.eglSwapBuffers) {
        fprintf(stderr, "Loader: CRITICAL FAILURE. Essential EGL functions could not be loaded.\\n");
        return -1;
    }
    return 0;
}
EOF

echo "--- Generation Complete ---"
echo "Generated files: $OUTPUT_HEADER and $OUTPUT_IMPL"
