#include "cache.h"
#include "gles.h"
#include "sha256.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

// --- Helper Functions ---

// Creates the cache directory if it doesn't exist.
static void ensure_cache_dir() {
    if (g_cache_dir[0] == '\0') {
        const char* home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "[Cache] HOME environment variable not set. Cannot create cache.\n");
            return;
        }
        snprintf(g_cache_dir, sizeof(g_cache_dir), "%s/.cache/my-gl-layer", home);
    }
    // Simple mkdir, not recursive. For a real app, you'd want to create .cache too.
    mkdir(g_cache_dir, 0755);
}

static void calculate_program_hash(GLuint program, char* out_hash_str) {
    GLint num_shaders = 0;
    gles.core.glGetProgramiv(program, GL_ATTACHED_SHADERS, &num_shaders);
    if (num_shaders == 0) {
        out_hash_str[0] = '\0';
        return;
    }

    GLuint* shaders = (GLuint*)malloc(num_shaders * sizeof(GLuint));
    if (!shaders) {
        fprintf(stderr, "[Cache] Failed to allocate memory for shader list.\n");
        out_hash_str[0] = '\0';
        return;
    }
    gles.core.glGetAttachedShaders(program, num_shaders, NULL, shaders);

    size_t total_source_len = 0;
    for (int i = 0; i < num_shaders; ++i) {
        char* source = hmget(g_shader_source_map, (uintptr_t)shaders[i]);
        if (source) {
            total_source_len += strlen(source);
        }
    }

    if (total_source_len == 0) {
        free(shaders);
        out_hash_str[0] = '\0';
        return;
    }

    char* concatenated_source = (char*)malloc(total_source_len + 1);
    if (!concatenated_source) {
        fprintf(stderr, "[Cache] Failed to allocate memory for concatenated sources.\n");
        free(shaders);
        out_hash_str[0] = '\0';
        return;
    }
    concatenated_source[0] = '\0'; // Start with an empty string for strcat

    for (int i = 0; i < num_shaders; ++i) {
        char* source = hmget(g_shader_source_map, (uintptr_t)shaders[i]);
        if (source) {
            strcat(concatenated_source, source);
        }
    }

    uint8_t hash[32];
    sha256((const uint8_t*)concatenated_source, total_source_len, hash);

    // Free the temporary concatenated buffer
    free(concatenated_source);
    free(shaders);

    // Convert binary hash to hex string (this part is the same as before)
    for (int i = 0; i < 32; ++i) {
        sprintf(out_hash_str + (i * 2), "%02x", hash[i]);
    }
    out_hash_str[32 * 2] = '\0';
}

void shader_cache_init() {
    printf("[Cache] Initializing shader cache system.\n");
    ensure_cache_dir();
}

void shader_cache_shutdown() {
    printf("[Cache] Shutting down shader cache system.\n");
    shfree(g_shader_source_map);
}

void shader_cache_add_source(GLuint shader, const GLchar* source) {
    char* source_copy = strdup(source);
    hmput(g_shader_source_map, (uintptr_t)shader, source_copy);
}

int shader_cache_load_program(GLuint program) {
    char hash_str[65];
    calculate_program_hash(program, hash_str);
    if (hash_str[0] == '\0') return 0;

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/%s", g_cache_dir, hash_str);

    FILE* f = fopen(file_path, "rb");
    if (!f) {
        printf("[Cache] MISS for program with hash %s\n", hash_str);
        return 0;
    }

    printf("[Cache] HIT for program with hash %s\n", hash_str);

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    GLenum binary_format;
    fread(&binary_format, sizeof(GLenum), 1, f);
    long binary_size = file_size - sizeof(GLenum);

    void* binary_data = malloc(binary_size);
    fread(binary_data, binary_size, 1, f);
    fclose(f);

    gles.core.glProgramBinary(program, binary_format, binary_data, binary_size);
    free(binary_data);

    GLint status;
    gles.core.glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        fprintf(stderr, "[Cache] Cached program failed to link (driver update?). Deleting cache entry.\n");
        remove(file_path);
        return 0; // Treat as a miss
    }

    return 1; // Success!
}

void shader_cache_save_program(GLuint program) {
    char hash_str[65];
    calculate_program_hash(program, hash_str);
    if (hash_str[0] == '\0') return;

    GLint binary_size = 0;
    gles.core.glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binary_size);
    if (binary_size == 0) return;

    void* binary_data = malloc(binary_size);
    GLenum binary_format;
    gles.core.glGetProgramBinary(program, binary_size, NULL, &binary_format, binary_data);

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s/%s", g_cache_dir, hash_str);

    FILE* f = fopen(file_path, "wb");
    if (!f) {
        fprintf(stderr, "[Cache] Failed to open %s for writing: %s\n", file_path, strerror(errno));
        free(binary_data);
        return;
    }

    fwrite(&binary_format, sizeof(GLenum), 1, f);
    fwrite(binary_data, binary_size, 1, f);
    fclose(f);

    printf("[Cache] SAVED program with hash %s\n", hash_str);
    free(binary_data);
}

void shader_cache_remove_program(GLuint program) {
    if (hmgeti(g_shader_source_map, program) >= 0) {
        free(hmget(g_shader_source_map, program));
        hmdel(g_shader_source_map, program);
    }
}
