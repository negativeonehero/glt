#ifndef SHADER_CACHE_H
#define SHADER_CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h> // For mkdir
#include <errno.h>

#include <GLES2/gl2.h>

typedef struct {
    uintptr_t key;
    char* value; // The source code string
} ShaderSourceEntry;

static ShaderSourceEntry* g_shader_source_map = NULL;
static char g_cache_dir[256];

void shader_cache_init();
void shader_cache_shutdown();

// Stores the original, unconverted source code for a shader.
void shader_cache_add_source(GLuint shader, const GLchar* source);

// Tries to load a program from cache. Returns 1 on success, 0 on miss.
int shader_cache_load_program(GLuint program);

// Saves a newly linked program to the cache.
void shader_cache_save_program(GLuint program);

// Remove an entry from the shader map.
void shader_cache_remove_program(GLuint program);

#endif // SHADER_CACHE_H
