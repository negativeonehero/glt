#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <GLES3/gl32.h>

#ifdef __cplusplus
extern "C" {
#endif

char* shader_translate(GLenum shader_type, const char* source);

#ifdef __cplusplus
}
#endif

#endif // TRANSLATE_H
