#include "state.h"
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

static struct {
    GLuint key;
    GLenum value;
} *g_texture_target_map = NULL;


void state_texture_set_target(GLuint texture, GLenum target) {
    if (texture == 0) return;
    hmput(g_texture_target_map, texture, target);
}

GLenum state_texture_get_target(GLuint texture) {
    if (texture == 0) return 0;
    return hmget(g_texture_target_map, texture);
}

void state_texture_remove(GLuint texture) {
    if (texture == 0) return;
    hmdel(g_texture_target_map, texture);
}

GLenum get_texture_binding_from_target(GLenum target) {
    switch (target) {
        case GL_TEXTURE_1D:
        case GL_TEXTURE_2D:
            return GL_TEXTURE_BINDING_2D;
        case GL_TEXTURE_2D_MULTISAMPLE:
            return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
        case GL_TEXTURE_2D_ARRAY:
            return GL_TEXTURE_BINDING_2D_ARRAY;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
        case GL_TEXTURE_3D:
            return GL_TEXTURE_BINDING_3D;
        case GL_TEXTURE_CUBE_MAP:
            return GL_TEXTURE_BINDING_CUBE_MAP;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
        case GL_TEXTURE_BUFFER:
            return GL_TEXTURE_BINDING_BUFFER;
        default:
            fprintf(stderr, "Warning: unknown texture target %#x in get_texture_binding_from_target\n", target);
            return GL_TEXTURE_BINDING_2D;
    }
}
