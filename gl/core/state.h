#ifndef STATE_H
#define STATE_H

#include "stb_ds.h"
#include <GLES3/gl32.h>

typedef struct {
    GLuint key;
    GLenum value;
} texture_target_map_t;

#define MAX_TEXTURE_UNITS 32

typedef struct {
    GLuint read_fbo;
    GLuint draw_fbo;
    GLuint array_buffer;
    GLuint element_array_buffer; // Also affects the active VAO's state
    GLuint copy_read_buffer;
    GLuint copy_write_buffer;
    GLuint uniform_buffer;
    // More buffer types...
    GLuint active_texture_unit;
    struct {
        GLuint texture_2d;
        GLuint texture_2d_multisample;
        GLuint texture_2d_array;
        GLuint texture_2d_multisample_array;
        GLuint texture_3d;
        GLuint texture_cube_map;
        GLuint texture_cube_map_array;
        GLuint texture_buffer;
    } texture_units[MAX_TEXTURE_UNITS];
    GLenum polygon_mode;
} gl_state_t;

extern gl_state_t gl_state;
extern texture_target_map_t* texture_target_map;

GLenum get_texture_target(GLuint texture);
GLenum get_gles_texture_target(GLenum original_target);
GLuint* get_cached_texture_binding(GLenum real_target, GLuint unit_index);

#endif
