#include "state.h"
#include <stdio.h>

gl_state_t gl_state = {0};
texture_target_map_t *texture_target_map = NULL;

GLenum get_texture_target(GLuint texture) {
    ptrdiff_t index = hmgeti(texture_target_map, texture);
    return texture_target_map[index].value;
}

GLenum get_gles_texture_target(GLenum original_target) {
    switch (original_target) {
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D_ARRAY:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_2D_MULTISAMPLE:
        case GL_TEXTURE_CUBE_MAP_ARRAY:
        case GL_TEXTURE_BUFFER:
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            return original_target;
        default:
            // An unknown or unsupported target.
            fprintf(stderr, "GLT WARN: Texture target %d not supported yet.", original_target);
            return GL_NONE;
    }
}

GLuint* get_cached_texture_binding(GLenum real_target, GLuint unit_index) {
    if (unit_index >= MAX_TEXTURE_UNITS) {
        return NULL; // Invalid unit index
    }

    switch (real_target) {
        case GL_TEXTURE_2D:			return &gl_state.texture_units[unit_index].texture_2d;
        case GL_TEXTURE_2D_MULTISAMPLE:		return &gl_state.texture_units[unit_index].texture_2d_multisample;
        case GL_TEXTURE_2D_ARRAY:		return &gl_state.texture_units[unit_index].texture_2d_array;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: 	return &gl_state.texture_units[unit_index].texture_2d_multisample_array;
        case GL_TEXTURE_3D:			return &gl_state.texture_units[unit_index].texture_3d;
        case GL_TEXTURE_CUBE_MAP:		return &gl_state.texture_units[unit_index].texture_cube_map;
        case GL_TEXTURE_CUBE_MAP_ARRAY:		return &gl_state.texture_units[unit_index].texture_cube_map_array;
        case GL_TEXTURE_BUFFER:			return &gl_state.texture_units[unit_index].texture_buffer;
        default:				return NULL;
    }
}
