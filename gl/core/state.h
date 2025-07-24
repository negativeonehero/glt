#ifndef STATE_H
#define STATE_H

#include <GL/glcorearb.h>

// Texture related functions, used to implement direct state access

void state_texture_set_target(GLuint texture, GLenum target);
GLenum state_texture_get_target(GLuint texture);
void state_texture_remove(GLuint texture);
GLenum get_texture_binding_from_target(GLenum target);

#endif // STATE_H
