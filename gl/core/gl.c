#include "gles.h"
#include "translate.h"
#include "cache.h"
#include "state.h"
#include <stdio.h>

#define UNIMPLEMENTED() \
    do { \
        static int warned = 0; \
        if (!warned) { \
            fprintf(stderr, "GL STUB: %s is not yet implemented!\n", __func__); \
            warned = 1; \
        } \
    } while (0)

#ifdef __cplusplus
extern "C" {
#endif

// Reusable implementations marked "internal"
void * glMapBufferRange_internal(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
    if ((access & GL_MAP_PERSISTENT_BIT_EXT) && !gles.ext.glBufferStorageEXT) {
        access &= ~(GL_MAP_PERSISTENT_BIT_EXT | GL_MAP_COHERENT_BIT_EXT);
    }

    return gles.core.glMapBufferRange(target, offset, length, access);
}

void * glMapBuffer_internal(GLenum target, GLenum access) {
    if(gles.ext.glMapBufferOES) return gles.ext.glMapBufferOES(target, access);
    GLint size;
    gles.core.glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
    GLbitfield access_flags = 0;
    if (access == GL_READ_ONLY)  access_flags = GL_MAP_READ_BIT;
    if (access == GL_WRITE_ONLY) access_flags = GL_MAP_WRITE_BIT;
    if (access == GL_READ_WRITE) access_flags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
    return gles.core.glMapBufferRange(target, 0, size, access_flags);
}

// GL API implementation
void glActiveShaderProgram(GLuint pipeline, GLuint program) {
    gles.core.glActiveShaderProgram(pipeline, program);
}

void glActiveTexture(GLenum texture) {
    gles.core.glActiveTexture(texture);
}

void glAttachShader(GLuint program, GLuint shader) {
    gles.core.glAttachShader(program, shader);
}

void glBeginConditionalRender(GLuint id, GLenum mode) {
    if(gles.ext.glBeginConditionalRenderNV) gles.ext.glBeginConditionalRenderNV(id, mode);
    else UNIMPLEMENTED();
}

void glBeginQuery(GLenum target, GLuint id) {
    gles.core.glBeginQuery(target, id);
}

void glBeginQueryIndexed(GLenum target, GLuint index, GLuint id) {
    UNIMPLEMENTED();
}

void glBeginTransformFeedback(GLenum primitiveMode) {
    gles.core.glBeginTransformFeedback(primitiveMode);
}

void glBindAttribLocation(GLuint program, GLuint index, const GLchar *name) {
    gles.core.glBindAttribLocation(program, index, name);
}

void glBindBuffer(GLenum target, GLuint buffer) {
    gles.core.glBindBuffer(target, buffer);
}

void glBindBufferBase(GLenum target, GLuint index, GLuint buffer) {
    gles.core.glBindBufferBase(target, index, buffer);
}

void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    gles.core.glBindBufferRange(target, index, buffer, offset, size);
}

void glBindBuffersBase(GLenum target, GLuint first, GLsizei count, const GLuint *buffers) {
    if(buffers == NULL) {
        for(GLsizei i = 0; i < count; ++i) {
            gles.core.glBindBufferBase(target, first + i, 0);
        }
    } else {
        for(GLsizei i = 0; i < count; ++i) {
            gles.core.glBindBufferBase(target, first + i, buffers[i]);
        }
    }
}

void glBindBuffersRange(GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes) {
    UNIMPLEMENTED();
}

void glBindFragDataLocation(GLuint program, GLuint color, const GLchar *name) {
    if(gles.ext.glBindFragDataLocationEXT) gles.ext.glBindFragDataLocationEXT(program, color, name);
    else UNIMPLEMENTED();
}

void glBindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name) {
    if(gles.ext.glBindFragDataLocationIndexedEXT) gles.ext.glBindFragDataLocationIndexedEXT(program, colorNumber, index, name);
    else UNIMPLEMENTED();
}

void glBindFramebuffer(GLenum target, GLuint framebuffer) {
    gles.core.glBindFramebuffer(target, framebuffer);
}

void glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format) {
    gles.core.glBindImageTexture(unit, texture, level, layered, layer, access, format);
}

void glBindImageTextures(GLuint first, GLsizei count, const GLuint *textures) {
    UNIMPLEMENTED();
}

void glBindProgramPipeline(GLuint pipeline) {
    gles.core.glBindProgramPipeline(pipeline);
}

void glBindRenderbuffer(GLenum target, GLuint renderbuffer) {
    gles.core.glBindRenderbuffer(target, renderbuffer);
}

void glBindSampler(GLuint unit, GLuint sampler) {
    gles.core.glBindSampler(unit, sampler);
}

void glBindSamplers(GLuint first, GLsizei count, const GLuint *samplers) {
    UNIMPLEMENTED();
}

void glBindTexture(GLenum target, GLuint texture) {
    state_texture_set_target(texture, target);
    gles.core.glBindTexture(target, texture);
}

void glBindTextureUnit(GLuint unit, GLuint texture) {
    UNIMPLEMENTED();
}

void glBindTextures(GLuint first, GLsizei count, const GLuint *textures) {
    UNIMPLEMENTED();
}

void glBindTransformFeedback(GLenum target, GLuint id) {
    gles.core.glBindTransformFeedback(target, id);
}

void glBindVertexArray(GLuint array) {
    gles.core.glBindVertexArray(array);
}

void glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
    gles.core.glBindVertexBuffer(bindingindex, buffer, offset, stride);
}

void glBindVertexBuffers(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides) {
    UNIMPLEMENTED();
}

void glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    gles.core.glBlendColor(red, green, blue, alpha);
}

void glBlendEquation(GLenum mode) {
    gles.core.glBlendEquation(mode);
}

void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {
    gles.core.glBlendEquationSeparate(modeRGB, modeAlpha);
}

void glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha) {
    gles.core.glBlendEquationSeparatei(buf, modeRGB, modeAlpha);
}

void glBlendEquationi(GLuint buf, GLenum mode) {
    gles.core.glBlendEquationi(buf, mode);
}

void glBlendFunc(GLenum sfactor, GLenum dfactor) {
    gles.core.glBlendFunc(sfactor, dfactor);
}

void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
    gles.core.glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

void glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
    gles.core.glBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void glBlendFunci(GLuint buf, GLenum src, GLenum dst) {
    gles.core.glBlendFunci(buf, src, dst);
}

void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    gles.core.glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

void glBlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    GLint old_read_fbo, old_draw_fbo;
    gles.core.glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &old_read_fbo);
    gles.core.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &old_draw_fbo);

    gles.core.glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer);
    gles.core.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer);

    gles.core.glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);

    gles.core.glBindFramebuffer(GL_READ_FRAMEBUFFER, old_read_fbo);
    gles.core.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, old_draw_fbo);
}

void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
    gles.core.glBufferData(target, size, data, usage);
}

void glBufferStorage(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags) {
    if(gles.ext.glBufferStorageEXT) gles.ext.glBufferStorageEXT(target, size, data, flags);
    else gles.core.glBufferData(target, size, data, GL_STATIC_DRAW);
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data) {
    gles.core.glBufferSubData(target, offset, size, data);
}

GLenum glCheckFramebufferStatus(GLenum target) {
    return gles.core.glCheckFramebufferStatus(target);
}

GLenum glCheckNamedFramebufferStatus(GLuint framebuffer, GLenum target) {
    const GLenum fbtarget = GL_FRAMEBUFFER;
    GLint old_fbo;
    gles.core.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
    gles.core.glBindFramebuffer(fbtarget, framebuffer);
    GLenum status = gles.core.glCheckFramebufferStatus(target);
    gles.core.glBindFramebuffer(fbtarget, old_fbo);
    return status;
}

void glClampColor(GLenum target, GLenum clamp) {
    UNIMPLEMENTED();
}

void glClear(GLbitfield mask) {
    gles.core.glClear(mask);
}

void glClearBufferData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data) {
    UNIMPLEMENTED();
}

void glClearBufferSubData(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) {
    UNIMPLEMENTED();
}

void glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
    gles.core.glClearBufferfi(buffer, drawbuffer, depth, stencil);
}

void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value) {
    gles.core.glClearBufferfv(buffer, drawbuffer, value);
}

void glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value) {
    gles.core.glClearBufferiv(buffer, drawbuffer, value);
}

void glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value) {
    gles.core.glClearBufferuiv(buffer, drawbuffer, value);
}

void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    gles.core.glClearColor(red, green, blue, alpha);
}

void glClearDepth(GLdouble depth) {
    gles.core.glClearDepthf((GLfloat)depth);
}

void glClearDepthf(GLfloat d) {
    gles.core.glClearDepthf(d);
}

void glClearNamedBufferData(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data) {
    UNIMPLEMENTED();
}

void glClearNamedBufferSubData(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data) {
    UNIMPLEMENTED();
}

void glClearNamedFramebufferfi(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) {
    UNIMPLEMENTED();
}

void glClearNamedFramebufferfv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value) {
    UNIMPLEMENTED();
}

void glClearNamedFramebufferiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value) {
    UNIMPLEMENTED();
}

void glClearNamedFramebufferuiv(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value) {
    UNIMPLEMENTED();
}

void glClearStencil(GLint s) {
    gles.core.glClearStencil(s);
}

void glClearTexImage(GLuint texture, GLint level, GLenum format, GLenum type, const void *data) {
    if(gles.ext.glClearTexImageEXT) gles.ext.glClearTexImageEXT(texture, level, format, type, data);
    else UNIMPLEMENTED();
}

void glClearTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) {
    if(gles.ext.glClearTexSubImageEXT) gles.ext.glClearTexSubImageEXT(texture, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data);
    else UNIMPLEMENTED();
}

GLenum glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) {
    return gles.core.glClientWaitSync(sync, flags, timeout);
}

void glClipControl(GLenum origin, GLenum depth) {
    if(gles.ext.glClipControlEXT) gles.ext.glClipControlEXT(origin, depth);
    else UNIMPLEMENTED();
}

void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
    gles.core.glColorMask(red, green, blue, alpha);
}

void glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) {
    gles.core.glColorMaski(index, r, g, b, a);
}

void glColorP3ui(GLenum type, GLuint color) {
    UNIMPLEMENTED();
}

void glColorP3uiv(GLenum type, const GLuint *color) {
    UNIMPLEMENTED();
}

void glColorP4ui(GLenum type, GLuint color) {
    UNIMPLEMENTED();
}

void glColorP4uiv(GLenum type, const GLuint *color) {
    UNIMPLEMENTED();
}

void glCompileShader(GLuint shader) {
    gles.core.glCompileShader(shader);
}

void glCompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data) {
    UNIMPLEMENTED();
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) {
    gles.core.glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) {
    gles.core.glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
}

void glCompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) {
    UNIMPLEMENTED();
}

void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
    gles.core.glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) {
    gles.core.glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void glCompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data) {
    UNIMPLEMENTED();
}

void glCompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
    UNIMPLEMENTED();
}

void glCompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) {
    UNIMPLEMENTED();
}

void glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    GLint is_read_mapped = GL_FALSE;
    gles.core.glGetBufferParameteriv(readTarget, GL_BUFFER_MAPPED, &is_read_mapped);

    if (is_read_mapped) {
        void* mapped_read_ptr = NULL;
        gles.core.glGetBufferPointerv(readTarget, GL_BUFFER_MAP_POINTER, &mapped_read_ptr);
        void* temp_buffer = malloc(size);
        memcpy(temp_buffer, (char*)mapped_read_ptr + readOffset, size);
        gles.core.glBufferSubData(writeTarget, writeOffset, size, temp_buffer);
        free(temp_buffer);
    } else {
        gles.core.glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    }
}

void glCopyImageSubData(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth) {
    gles.core.glCopyImageSubData(srcName, srcTarget, srcLevel, srcX, srcY, srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth, srcHeight, srcDepth);
}

void glCopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_readBuffer, old_writeBuffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_readBuffer);
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_writeBuffer);
    gles.core.glBindBuffer(target, readBuffer);
    gles.core.glBindBuffer(target, writeBuffer);
    gles.core.glCopyBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
    gles.core.glBindBuffer(target, old_readBuffer);
    gles.core.glBindBuffer(target, old_writeBuffer);
}

void glCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) {
    UNIMPLEMENTED();
}

void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
    gles.core.glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
    UNIMPLEMENTED();
}

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    gles.core.glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    gles.core.glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
}

void glCopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) {
    UNIMPLEMENTED();
}

void glCopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    UNIMPLEMENTED();
}

void glCopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
    UNIMPLEMENTED();
}

void glCreateBuffers(GLsizei n, GLuint *buffers) {
    gles.core.glGenBuffers(n, buffers);
}

void glCreateFramebuffers(GLsizei n, GLuint *framebuffers) {
    gles.core.glGenFramebuffers(n, framebuffers);
}

GLuint glCreateProgram(void) {
    return gles.core.glCreateProgram();
}

void glCreateProgramPipelines(GLsizei n, GLuint *pipelines) {
    gles.core.glGenProgramPipelines(n, pipelines);
}

void glCreateQueries(GLenum target, GLsizei n, GLuint *ids) {
    // GLES glGenQueries doesn't have a target
    gles.core.glGenQueries(n, ids);
}

void glCreateRenderbuffers(GLsizei n, GLuint *renderbuffers) {
    gles.core.glGenRenderbuffers(n, renderbuffers);
}

void glCreateSamplers(GLsizei n, GLuint *samplers) {
    gles.core.glGenSamplers(n, samplers);
}

GLuint glCreateShader(GLenum type) {
    return gles.core.glCreateShader(type);
}

GLuint glCreateShaderProgramv(GLenum type, GLsizei count, const GLchar *const *strings) {
    return gles.core.glCreateShaderProgramv(type, count, strings);
}

void glCreateTextures(GLenum target, GLsizei n, GLuint *textures) {
    gles.core.glGenTextures(n, textures);
    for (GLsizei i = 0; i < n; ++i) {
        state_texture_set_target(textures[i], target);
    }
}

void glCreateTransformFeedbacks(GLsizei n, GLuint *ids) {
    gles.core.glGenTransformFeedbacks(n, ids);
}

void glCreateVertexArrays(GLsizei n, GLuint *arrays) {
    gles.core.glGenVertexArrays(n, arrays);
}

void glCullFace(GLenum mode) {
    gles.core.glCullFace(mode);
}

void glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam) {
    gles.core.glDebugMessageCallback(callback, userParam);
}

void glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled) {
    gles.core.glDebugMessageControl(source, type, severity, count, ids, enabled);
}

void glDebugMessageInsert(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf) {
    gles.core.glDebugMessageInsert(source, type, id, severity, length, buf);
}

void glDeleteBuffers(GLsizei n, const GLuint *buffers) {
    gles.core.glDeleteBuffers(n, buffers);
}

void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers) {
    gles.core.glDeleteFramebuffers(n, framebuffers);
}

void glDeleteProgram(GLuint program) {
    gles.core.glDeleteProgram(program);
}

void glDeleteProgramPipelines(GLsizei n, const GLuint *pipelines) {
    UNIMPLEMENTED();
}

void glDeleteQueries(GLsizei n, const GLuint *ids) {
    gles.core.glDeleteQueries(n, ids);
}

void glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers) {
    gles.core.glDeleteRenderbuffers(n, renderbuffers);
}

void glDeleteSamplers(GLsizei count, const GLuint *samplers) {
    gles.core.glDeleteSamplers(count, samplers);
}

void glDeleteShader(GLuint shader) {
    shader_cache_remove_program(shader);
    gles.core.glDeleteShader(shader);
}

void glDeleteSync(GLsync sync) {
    gles.core.glDeleteSync(sync);
}

void glDeleteTextures(GLsizei n, const GLuint *textures) {
    if (!textures) return;
    for (GLsizei i = 0; i < n; ++i) {
        state_texture_remove(textures[i]);
    }
    gles.core.glDeleteTextures(n, textures);
}

void glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids) {
    gles.core.glDeleteTransformFeedbacks(n, ids);
}

void glDeleteVertexArrays(GLsizei n, const GLuint *arrays) {
    gles.core.glDeleteVertexArrays(n, arrays);
}

void glDepthFunc(GLenum func) {
    gles.core.glDepthFunc(func);
}

void glDepthMask(GLboolean flag) {
    gles.core.glDepthMask(flag);
}

void glDepthRange(GLdouble n, GLdouble f) {
    UNIMPLEMENTED();
}

void glDepthRangeArrayv(GLuint first, GLsizei count, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glDepthRangeIndexed(GLuint index, GLdouble n, GLdouble f) {
    UNIMPLEMENTED();
}

void glDepthRangef(GLfloat n, GLfloat f) {
    gles.core.glDepthRangef(n, f);
}

void glDetachShader(GLuint program, GLuint shader) {
    gles.core.glDetachShader(program, shader);
}

void glDisable(GLenum cap) {
    gles.core.glDisable(cap);
}

void glDisableVertexArrayAttrib(GLuint vaobj, GLuint index) {
    UNIMPLEMENTED();
}

void glDisableVertexAttribArray(GLuint index) {
    gles.core.glDisableVertexAttribArray(index);
}

void glDisablei(GLenum target, GLuint index) {
    gles.core.glDisablei(target, index);
}

void glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z) {
    gles.core.glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void glDispatchComputeIndirect(GLintptr indirect) {
    gles.core.glDispatchComputeIndirect(indirect);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
    gles.core.glDrawArrays(mode, first, count);
}

void glDrawArraysIndirect(GLenum mode, const void *indirect) {
    gles.core.glDrawArraysIndirect(mode, indirect);
}

void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instancecount) {
    gles.core.glDrawArraysInstanced(mode, first, count, instancecount);
}

void glDrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance) {
    if(gles.ext.glDrawArraysInstancedBaseInstanceEXT) gles.ext.glDrawArraysInstancedBaseInstanceEXT(mode, first, count, instancecount, baseinstance);
    else UNIMPLEMENTED();
}

void glDrawBuffer(GLenum buf) {
    const GLenum bufs[] = { buf };
    gles.core.glDrawBuffers(1, bufs);
}

void glDrawBuffers(GLsizei n, const GLenum *bufs) {
    gles.core.glDrawBuffers(n, bufs);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    gles.core.glDrawElements(mode, count, type, indices);
}

void glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) {
    gles.core.glDrawElementsBaseVertex(mode, count, type, indices, basevertex);
}

void glDrawElementsIndirect(GLenum mode, GLenum type, const void *indirect) {
    gles.core.glDrawElementsIndirect(mode, type, indirect);
}

void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount) {
    gles.core.glDrawElementsInstanced(mode, count, type, indices, instancecount);
}

void glDrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance) {
    if(gles.ext.glDrawElementsInstancedBaseInstanceEXT) gles.ext.glDrawElementsInstancedBaseInstanceEXT(mode, count, type, indices, instancecount, baseinstance);
    else UNIMPLEMENTED();
}

void glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex) {
    gles.core.glDrawElementsInstancedBaseVertex(mode, count, type, indices, instancecount, basevertex);
}

void glDrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance) {
    if(gles.ext.glDrawElementsInstancedBaseVertexBaseInstanceEXT) gles.ext.glDrawElementsInstancedBaseVertexBaseInstanceEXT(mode, count, type, indices, instancecount, basevertex, baseinstance);
    else UNIMPLEMENTED();
}

void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices) {
    gles.core.glDrawRangeElements(mode, start, end, count, type, indices);
}

void glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex) {
    gles.core.glDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
}

void glDrawTransformFeedback(GLenum mode, GLuint id) {
    if(gles.ext.glDrawTransformFeedbackEXT) gles.ext.glDrawTransformFeedbackEXT(mode, id);
    else UNIMPLEMENTED();
}

void glDrawTransformFeedbackInstanced(GLenum mode, GLuint id, GLsizei instancecount) {
    if(gles.ext.glDrawTransformFeedbackInstancedEXT) gles.ext.glDrawTransformFeedbackInstancedEXT(mode, id, instancecount);
    else UNIMPLEMENTED();
}

void glDrawTransformFeedbackStream(GLenum mode, GLuint id, GLuint stream) {
    UNIMPLEMENTED();
}

void glDrawTransformFeedbackStreamInstanced(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount) {
    UNIMPLEMENTED();
}

void glEnable(GLenum cap) {
    gles.core.glEnable(cap);
}

void glEnableVertexArrayAttrib(GLuint vaobj, GLuint index) {
    GLint old_vao;
    gles.core.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_vao);
    gles.core.glBindVertexArray(vaobj);
    gles.core.glEnableVertexAttribArray(index);
    gles.core.glBindVertexArray(old_vao);
}

void glEnableVertexAttribArray(GLuint index) {
    gles.core.glEnableVertexAttribArray(index);
}

void glEnablei(GLenum target, GLuint index) {
    gles.core.glEnablei(target, index);
}

void glEndConditionalRender(void) {
    if(gles.ext.glEndConditionalRenderNV) gles.ext.glEndConditionalRenderNV();
    else UNIMPLEMENTED();
}

void glEndQuery(GLenum target) {
    gles.core.glEndQuery(target);
}

void glEndQueryIndexed(GLenum target, GLuint index) {
    UNIMPLEMENTED();
}

void glEndTransformFeedback(void) {
    gles.core.glEndTransformFeedback();
}

GLsync glFenceSync(GLenum condition, GLbitfield flags) {
    return gles.core.glFenceSync(condition, flags);
}

void glFinish(void) {
    gles.core.glFinish();
}

void glFlush(void) {
    gles.core.glFlush();
}

void glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length) {
    gles.core.glFlushMappedBufferRange(target, offset, length);
}

void glFlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    gles.core.glFlushMappedBufferRange(target, offset, length);
    gles.core.glBindBuffer(target, old_buffer);
}

void glFramebufferParameteri(GLenum target, GLenum pname, GLint param) {
    gles.core.glFramebufferParameteri(target, pname, param);
}

void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    gles.core.glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level) {
    gles.core.glFramebufferTexture(target, attachment, texture, level);
}

void glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    UNIMPLEMENTED();
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {
    gles.core.glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) {
    if(gles.ext.glFramebufferTexture3DOES) gles.ext.glFramebufferTexture3DOES(target, attachment, textarget, texture, level, zoffset);
    else UNIMPLEMENTED();
}

void glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) {
    gles.core.glFramebufferTextureLayer(target, attachment, texture, level, layer);
}

void glFrontFace(GLenum mode) {
    gles.core.glFrontFace(mode);
}

void glGenBuffers(GLsizei n, GLuint *buffers) {
    gles.core.glGenBuffers(n, buffers);
}

void glGenFramebuffers(GLsizei n, GLuint *framebuffers) {
    gles.core.glGenFramebuffers(n, framebuffers);
}

void glGenProgramPipelines(GLsizei n, GLuint *pipelines) {
    gles.core.glGenProgramPipelines(n, pipelines);
}

void glGenQueries(GLsizei n, GLuint *ids) {
    gles.core.glGenQueries(n, ids);
}

void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers) {
    gles.core.glGenRenderbuffers(n, renderbuffers);
}

void glGenSamplers(GLsizei count, GLuint *samplers) {
    gles.core.glGenSamplers(count, samplers);
}

void glGenTextures(GLsizei n, GLuint *textures) {
    gles.core.glGenTextures(n, textures);
}

void glGenTransformFeedbacks(GLsizei n, GLuint *ids) {
    gles.core.glGenTransformFeedbacks(n, ids);
}

void glGenVertexArrays(GLsizei n, GLuint *arrays) {
    gles.core.glGenVertexArrays(n, arrays);
}

void glGenerateMipmap(GLenum target) {
    gles.core.glGenerateMipmap(target);
}

void glGenerateTextureMipmap(GLuint texture) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glGenerateMipmap(target);
    gles.core.glBindTexture(target, old_texture);
}

void glGetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params) {
    UNIMPLEMENTED();
}

void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
    gles.core.glGetActiveAttrib(program, index, bufSize, length, size, type, name);
}

void glGetActiveSubroutineName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name) {
    UNIMPLEMENTED();
}

void glGetActiveSubroutineUniformName(GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name) {
    UNIMPLEMENTED();
}

void glGetActiveSubroutineUniformiv(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values) {
    UNIMPLEMENTED();
}

void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) {
    gles.core.glGetActiveUniform(program, index, bufSize, length, size, type, name);
}

void glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName) {
    gles.core.glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
}

void glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params) {
    gles.core.glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
}

void glGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName) {
    UNIMPLEMENTED();
}

void glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params) {
    gles.core.glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
}

void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) {
    gles.core.glGetAttachedShaders(program, maxCount, count, shaders);
}

GLint glGetAttribLocation(GLuint program, const GLchar *name) {
    return gles.core.glGetAttribLocation(program, name);
}

void glGetBooleani_v(GLenum target, GLuint index, GLboolean *data) {
    gles.core.glGetBooleani_v(target, index, data);
}

void glGetBooleanv(GLenum pname, GLboolean *data) {
    gles.core.glGetBooleanv(pname, data);
}

void glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params) {
    gles.core.glGetBufferParameteri64v(target, pname, params);
}

void glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params) {
    gles.core.glGetBufferParameteriv(target, pname, params);
}

void glGetBufferPointerv(GLenum target, GLenum pname, void **params) {
    gles.core.glGetBufferPointerv(target, pname, params);
}

void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void *data) {
    UNIMPLEMENTED();
}

void glGetCompressedTexImage(GLenum target, GLint level, void *img) {
    UNIMPLEMENTED();
}

void glGetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, void *pixels) {
    UNIMPLEMENTED();
}

void glGetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels) {
    UNIMPLEMENTED();
}

GLuint glGetDebugMessageLog(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog) {
    return gles.core.glGetDebugMessageLog(count, bufSize, sources, types, ids, severities, lengths, messageLog);
}

void glGetDoublei_v(GLenum target, GLuint index, GLdouble *data) {
    UNIMPLEMENTED();
}

void glGetDoublev(GLenum pname, GLdouble *data) {
    UNIMPLEMENTED();
}

GLenum glGetError(void) {
    return gles.core.glGetError();
}

void glGetFloati_v(GLenum target, GLuint index, GLfloat *data) {
    UNIMPLEMENTED();
}

void glGetFloatv(GLenum pname, GLfloat *data) {
    gles.core.glGetFloatv(pname, data);
}

GLint glGetFragDataIndex(GLuint program, const GLchar *name) {
    UNIMPLEMENTED();
    return 0; // FIXME: Add a proper return value!
}

GLint glGetFragDataLocation(GLuint program, const GLchar *name) {
    return gles.core.glGetFragDataLocation(program, name);
}

void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params) {
    gles.core.glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void glGetFramebufferParameteriv(GLenum target, GLenum pname, GLint *params) {
    gles.core.glGetFramebufferParameteriv(target, pname, params);
}

GLenum glGetGraphicsResetStatus(void) {
    return gles.core.glGetGraphicsResetStatus();
}

void glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data) {
    gles.core.glGetInteger64i_v(target, index, data);
}

void glGetInteger64v(GLenum pname, GLint64 *data) {
    gles.core.glGetInteger64v(pname, data);
}

void glGetIntegeri_v(GLenum target, GLuint index, GLint *data) {
    gles.core.glGetIntegeri_v(target, index, data);
}

void glGetIntegerv(GLenum pname, GLint *data) {
    gles.core.glGetIntegerv(pname, data);
}

void glGetInternalformati64v(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64 *params) {
    UNIMPLEMENTED();
}

void glGetInternalformativ(GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint *params) {
    gles.core.glGetInternalformativ(target, internalformat, pname, count, params);
}

void glGetMultisamplefv(GLenum pname, GLuint index, GLfloat *val) {
    gles.core.glGetMultisamplefv(pname, index, val);
}

void glGetNamedBufferParameteri64v(GLuint buffer, GLenum pname, GLint64 *params) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    gles.core.glGetBufferParameteri64v(buffer, pname, params);
    gles.core.glBindBuffer(target, old_buffer);
}

void glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint *params) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    gles.core.glGetBufferParameteriv(buffer, pname, params);
    gles.core.glBindBuffer(target, old_buffer);
}

void glGetNamedBufferPointerv(GLuint buffer, GLenum pname, void **params) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    gles.core.glGetBufferPointerv(buffer, pname, params);
    gles.core.glBindBuffer(target, old_buffer);
}

void glGetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, void *data) {
    UNIMPLEMENTED();
}

void glGetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params) {
    UNIMPLEMENTED();
}

void glGetNamedFramebufferParameteriv(GLuint framebuffer, GLenum pname, GLint *param) {
    UNIMPLEMENTED();
}

void glGetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum pname, GLint *params) {
    UNIMPLEMENTED();
}

void glGetObjectLabel(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label) {
    gles.core.glGetObjectLabel(identifier, name, bufSize, length, label);
}

void glGetObjectPtrLabel(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label) {
    gles.core.glGetObjectPtrLabel(ptr, bufSize, length, label);
}

void glGetPointerv(GLenum pname, void **params) {
    gles.core.glGetPointerv(pname, params);
}

void glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) {
    gles.core.glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
}

void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    gles.core.glGetProgramInfoLog(program, bufSize, length, infoLog);
}

void glGetProgramInterfaceiv(GLuint program, GLenum programInterface, GLenum pname, GLint *params) {
    gles.core.glGetProgramInterfaceiv(program, programInterface, pname, params);
}

void glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    gles.core.glGetProgramPipelineInfoLog(pipeline, bufSize, length, infoLog);
}

void glGetProgramPipelineiv(GLuint pipeline, GLenum pname, GLint *params) {
    gles.core.glGetProgramPipelineiv(pipeline, pname, params);
}

GLuint glGetProgramResourceIndex(GLuint program, GLenum programInterface, const GLchar *name) {
    return gles.core.glGetProgramResourceIndex(program, programInterface, name);
}

GLint glGetProgramResourceLocation(GLuint program, GLenum programInterface, const GLchar *name) {
    return gles.core.glGetProgramResourceLocation(program, programInterface, name);
}

GLint glGetProgramResourceLocationIndex(GLuint program, GLenum programInterface, const GLchar *name) {
    if(gles.ext.glGetProgramResourceLocationIndexEXT) return gles.ext.glGetProgramResourceLocationIndexEXT(program, programInterface, name);
    else { UNIMPLEMENTED(); return 0; }
}

void glGetProgramResourceName(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name) {
    gles.core.glGetProgramResourceName(program, programInterface, index, bufSize, length, name);
}

void glGetProgramResourceiv(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei count, GLsizei *length, GLint *params) {
    gles.core.glGetProgramResourceiv(program, programInterface, index, propCount, props, count, length, params);
}

void glGetProgramStageiv(GLuint program, GLenum shadertype, GLenum pname, GLint *values) {
    UNIMPLEMENTED();
}

void glGetProgramiv(GLuint program, GLenum pname, GLint *params) {
    gles.core.glGetProgramiv(program, pname, params);
}

void glGetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    UNIMPLEMENTED();
}

void glGetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    UNIMPLEMENTED();
}

void glGetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    UNIMPLEMENTED();
}

void glGetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum pname, GLintptr offset) {
    UNIMPLEMENTED();
}

void glGetQueryIndexediv(GLenum target, GLuint index, GLenum pname, GLint *params) {
    UNIMPLEMENTED();
}

void glGetQueryObjecti64v(GLuint id, GLenum pname, GLint64 *params) {
    if (gles.ext.glGetQueryObjecti64vEXT) gles.ext.glGetQueryObjecti64vEXT(id, pname, params);
    else UNIMPLEMENTED();
}

void glGetQueryObjectiv(GLuint id, GLenum pname, GLint *params) {
    if (gles.ext.glGetQueryObjectivEXT) gles.ext.glGetQueryObjectivEXT(id, pname, params);
    else UNIMPLEMENTED();
}

void glGetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 *params) {
    if (gles.ext.glGetQueryObjectui64vEXT) gles.ext.glGetQueryObjectui64vEXT(id, pname, params);
    else UNIMPLEMENTED();
}

void glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params) {
    if (gles.ext.glGetQueryObjectuivEXT) gles.ext.glGetQueryObjectuivEXT(id, pname, params);
    else UNIMPLEMENTED();
}

void glGetQueryiv(GLenum target, GLenum pname, GLint *params) {
    gles.core.glGetQueryiv(target, pname, params);
}

void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params) {
    gles.core.glGetRenderbufferParameteriv(target, pname, params);
}

void glGetSamplerParameterIiv(GLuint sampler, GLenum pname, GLint *params) {
    gles.core.glGetSamplerParameterIiv(sampler, pname, params);
}

void glGetSamplerParameterIuiv(GLuint sampler, GLenum pname, GLuint *params) {
    gles.core.glGetSamplerParameterIuiv(sampler, pname, params);
}

void glGetSamplerParameterfv(GLuint sampler, GLenum pname, GLfloat *params) {
    gles.core.glGetSamplerParameterfv(sampler, pname, params);
}

void glGetSamplerParameteriv(GLuint sampler, GLenum pname, GLint *params) {
    gles.core.glGetSamplerParameteriv(sampler, pname, params);
}

void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    gles.core.glGetShaderInfoLog(shader, bufSize, length, infoLog);
}

void glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) {
    gles.core.glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) {
    gles.core.glGetShaderSource(shader, bufSize, length, source);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
    gles.core.glGetShaderiv(shader, pname, params);
}

const GLubyte* glGetString(GLenum name) {
    static char version_str[512];
    static char renderer_str[512];
    static char vendor_str[512];
    static char sl_version_str[512];

    switch (name) {
        case GL_VERSION: {
            const char* gles_version = (const char*)gles.core.glGetString(GL_VERSION);
            snprintf(version_str, sizeof(version_str), "4.6 Core Profile | %s", gles_version);

            return (const GLubyte*)version_str;
        }

        case GL_RENDERER: {
            const char* gles_renderer = (const char*)gles.core.glGetString(GL_RENDERER);
            snprintf(renderer_str, sizeof(renderer_str), "glt 1.0.0 | %s", gles_renderer);
            return (const GLubyte*)renderer_str;
        }

        case GL_VENDOR: {
            const char* gles_vendor = (const char*)gles.core.glGetString(GL_VENDOR);
            snprintf(vendor_str, sizeof(vendor_str), "NegativeOneHero, RBRStan, %s", gles_vendor);
            return (const GLubyte*)vendor_str;
        }

        case GL_SHADING_LANGUAGE_VERSION: {
            const char* gles_sl_version = (const char*)gles.core.glGetString(GL_SHADING_LANGUAGE_VERSION);
            snprintf(sl_version_str, sizeof(sl_version_str), "4.60 | %s", gles_sl_version);
            return (const GLubyte*)sl_version_str;
        }

        default:
            return gles.core.glGetString(name);
    }
}

const GLubyte * glGetStringi(GLenum name, GLuint index) {
    return gles.core.glGetStringi(name, index);
}

GLuint glGetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar *name) {
    UNIMPLEMENTED();
    return 0; // FIXME: Add a proper return value!
}

GLint glGetSubroutineUniformLocation(GLuint program, GLenum shadertype, const GLchar *name) {
    UNIMPLEMENTED();
    return 0; // FIXME: Add a proper return value!
}

void glGetSynciv(GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values) {
    gles.core.glGetSynciv(sync, pname, count, length, values);
}

void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void *pixels) {
    UNIMPLEMENTED();
}

void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params) {
    gles.core.glGetTexLevelParameterfv(target, level, pname, params);
}

void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params) {
    gles.core.glGetTexLevelParameteriv(target, level, pname, params);
}

void glGetTexParameterIiv(GLenum target, GLenum pname, GLint *params) {
    gles.core.glGetTexParameterIiv(target, pname, params);
}

void glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params) {
    gles.core.glGetTexParameterIuiv(target, pname, params);
}

void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params) {
    gles.core.glGetTexParameterfv(target, pname, params);
}

void glGetTexParameteriv(GLenum target, GLenum pname, GLint *params) {
    gles.core.glGetTexParameteriv(target, pname, params);
}

void glGetTextureImage(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels) {
    UNIMPLEMENTED();
}

void glGetTextureLevelParameterfv(GLuint texture, GLint level, GLenum pname, GLfloat *params) {
    UNIMPLEMENTED();
}

void glGetTextureLevelParameteriv(GLuint texture, GLint level, GLenum pname, GLint *params) {
    UNIMPLEMENTED();
}

void glGetTextureParameterIiv(GLuint texture, GLenum pname, GLint *params) {
    UNIMPLEMENTED();
}

void glGetTextureParameterIuiv(GLuint texture, GLenum pname, GLuint *params) {
    UNIMPLEMENTED();
}

void glGetTextureParameterfv(GLuint texture, GLenum pname, GLfloat *params) {
    UNIMPLEMENTED();
}

void glGetTextureParameteriv(GLuint texture, GLenum pname, GLint *params) {
    UNIMPLEMENTED();
}

void glGetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels) {
    UNIMPLEMENTED();
}

void glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) {
    gles.core.glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
}

void glGetTransformFeedbacki64_v(GLuint xfb, GLenum pname, GLuint index, GLint64 *param) {
    UNIMPLEMENTED();
}

void glGetTransformFeedbacki_v(GLuint xfb, GLenum pname, GLuint index, GLint *param) {
    UNIMPLEMENTED();
}

void glGetTransformFeedbackiv(GLuint xfb, GLenum pname, GLint *param) {
    UNIMPLEMENTED();
}

GLuint glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName) {
    return gles.core.glGetUniformBlockIndex(program, uniformBlockName);
}

void glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const *uniformNames, GLuint *uniformIndices) {
    gles.core.glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
}

GLint glGetUniformLocation(GLuint program, const GLchar *name) {
    return gles.core.glGetUniformLocation(program, name);
}

void glGetUniformSubroutineuiv(GLenum shadertype, GLint location, GLuint *params) {
    UNIMPLEMENTED();
}

void glGetUniformdv(GLuint program, GLint location, GLdouble *params) {
    UNIMPLEMENTED();
}

void glGetUniformfv(GLuint program, GLint location, GLfloat *params) {
    gles.core.glGetUniformfv(program, location, params);
}

void glGetUniformiv(GLuint program, GLint location, GLint *params) {
    gles.core.glGetUniformiv(program, location, params);
}

void glGetUniformuiv(GLuint program, GLint location, GLuint *params) {
    gles.core.glGetUniformuiv(program, location, params);
}

void glGetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum pname, GLint64 *param) {
    UNIMPLEMENTED();
}

void glGetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum pname, GLint *param) {
    UNIMPLEMENTED();
}

void glGetVertexArrayiv(GLuint vaobj, GLenum pname, GLint *param) {
    UNIMPLEMENTED();
}

void glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params) {
    gles.core.glGetVertexAttribIiv(index, pname, params);
}

void glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params) {
    gles.core.glGetVertexAttribIuiv(index, pname, params);
}

void glGetVertexAttribLdv(GLuint index, GLenum pname, GLdouble *params) {
    UNIMPLEMENTED();
}

void glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer) {
    gles.core.glGetVertexAttribPointerv(index, pname, pointer);
}

void glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params) {
    UNIMPLEMENTED();
}

void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) {
    gles.core.glGetVertexAttribfv(index, pname, params);
}

void glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) {
    gles.core.glGetVertexAttribiv(index, pname, params);
}

void glGetnColorTable(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table) {
    UNIMPLEMENTED();
}

void glGetnCompressedTexImage(GLenum target, GLint lod, GLsizei bufSize, void *pixels) {
    UNIMPLEMENTED();
}

void glGetnConvolutionFilter(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image) {
    UNIMPLEMENTED();
}

void glGetnHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) {
    UNIMPLEMENTED();
}

void glGetnMapdv(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v) {
    UNIMPLEMENTED();
}

void glGetnMapfv(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v) {
    UNIMPLEMENTED();
}

void glGetnMapiv(GLenum target, GLenum query, GLsizei bufSize, GLint *v) {
    UNIMPLEMENTED();
}

void glGetnMinmax(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values) {
    UNIMPLEMENTED();
}

void glGetnPixelMapfv(GLenum map, GLsizei bufSize, GLfloat *values) {
    UNIMPLEMENTED();
}

void glGetnPixelMapuiv(GLenum map, GLsizei bufSize, GLuint *values) {
    UNIMPLEMENTED();
}

void glGetnPixelMapusv(GLenum map, GLsizei bufSize, GLushort *values) {
    UNIMPLEMENTED();
}

void glGetnPolygonStipple(GLsizei bufSize, GLubyte *pattern) {
    UNIMPLEMENTED();
}

void glGetnSeparableFilter(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span) {
    UNIMPLEMENTED();
}

void glGetnTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels) {
    UNIMPLEMENTED();
}

void glGetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params) {
    UNIMPLEMENTED();
}

void glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params) {
    gles.core.glGetnUniformfv(program, location, bufSize, params);
}

void glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params) {
    gles.core.glGetnUniformiv(program, location, bufSize, params);
}

void glGetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params) {
    gles.core.glGetnUniformuiv(program, location, bufSize, params);
}

void glHint(GLenum target, GLenum mode) {
    gles.core.glHint(target, mode);
}

void glInvalidateBufferData(GLuint buffer) {
    UNIMPLEMENTED();
}

void glInvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length) {
    UNIMPLEMENTED();
}

void glInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments) {
    gles.core.glInvalidateFramebuffer(target, numAttachments, attachments);
}

void glInvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments) {
    UNIMPLEMENTED();
}

void glInvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) {
    UNIMPLEMENTED();
}

void glInvalidateSubFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height) {
    gles.core.glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
}

void glInvalidateTexImage(GLuint texture, GLint level) {
    UNIMPLEMENTED();
}

void glInvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth) {
    UNIMPLEMENTED();
}

GLboolean glIsBuffer(GLuint buffer) {
    return gles.core.glIsBuffer(buffer);
}

GLboolean glIsEnabled(GLenum cap) {
    return gles.core.glIsEnabled(cap);
}

GLboolean glIsEnabledi(GLenum target, GLuint index) {
    return gles.core.glIsEnabledi(target, index);
}

GLboolean glIsFramebuffer(GLuint framebuffer) {
    return gles.core.glIsFramebuffer(framebuffer);
}

GLboolean glIsProgram(GLuint program) {
    return gles.core.glIsProgram(program);
}

GLboolean glIsProgramPipeline(GLuint pipeline) {
    return gles.core.glIsProgramPipeline(pipeline);
}

GLboolean glIsQuery(GLuint id) {
    return gles.core.glIsQuery(id);
}

GLboolean glIsRenderbuffer(GLuint renderbuffer) {
    return gles.core.glIsRenderbuffer(renderbuffer);
}

GLboolean glIsSampler(GLuint sampler) {
    return gles.core.glIsSampler(sampler);
}

GLboolean glIsShader(GLuint shader) {
    return gles.core.glIsShader(shader);
}

GLboolean glIsSync(GLsync sync) {
    return gles.core.glIsSync(sync);
}

GLboolean glIsTexture(GLuint texture) {
    return gles.core.glIsTexture(texture);
}

GLboolean glIsTransformFeedback(GLuint id) {
    return gles.core.glIsTransformFeedback(id);
}

GLboolean glIsVertexArray(GLuint array) {
    return gles.core.glIsVertexArray(array);
}

void glLineWidth(GLfloat width) {
    gles.core.glLineWidth(width);
}

void glLinkProgram(GLuint program) {
    if (shader_cache_load_program(program)) {
        return;
    }
    gles.core.glLinkProgram(program);
    GLint status;
    gles.core.glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_TRUE) {
        shader_cache_save_program(program);
    }
}

void glLogicOp(GLenum opcode) {
    UNIMPLEMENTED();
}

void * glMapBuffer(GLenum target, GLenum access) {
    return glMapBuffer_internal(target, access);
}

void* glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) {
    return glMapBufferRange_internal(target, offset, length, access);
}

void * glMapNamedBuffer(GLuint buffer, GLenum access) {
    const GLenum target = GL_ARRAY_BUFFER;
    void* mapped_ptr = NULL;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    mapped_ptr = glMapBuffer_internal(target, access);
    gles.core.glBindBuffer(target, old_buffer);
    return mapped_ptr;
}

void * glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access) {
    const GLenum target = GL_ARRAY_BUFFER;
    void* mapped_ptr = NULL;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    mapped_ptr = glMapBufferRange_internal(target, offset, length, access);
    gles.core.glBindBuffer(target, old_buffer);
    return mapped_ptr;
}

void glMemoryBarrier(GLbitfield barriers) {
    gles.core.glMemoryBarrier(barriers);
}

void glMemoryBarrierByRegion(GLbitfield barriers) {
    gles.core.glMemoryBarrierByRegion(barriers);
}

void glMinSampleShading(GLfloat value) {
    gles.core.glMinSampleShading(value);
}

void glMultiDrawArrays(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) {
    if(gles.ext.glMultiDrawArraysEXT) gles.ext.glMultiDrawArraysEXT(mode, first, count, drawcount);
    else UNIMPLEMENTED();
}

void glMultiDrawArraysIndirect(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride) {
    if(gles.ext.glMultiDrawArraysIndirectEXT) gles.ext.glMultiDrawArraysIndirectEXT(mode, indirect, drawcount, stride);
    else UNIMPLEMENTED();
}

void glMultiDrawArraysIndirectCount(GLenum mode, const void *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
    UNIMPLEMENTED();
}

void glMultiDrawElements(GLenum mode, const GLsizei *count, GLenum type, const void *const *indices, GLsizei drawcount) {
    if(gles.ext.glMultiDrawElementsEXT) gles.ext.glMultiDrawElementsEXT(mode, count, type, indices, drawcount);
    else UNIMPLEMENTED();
}

void glMultiDrawElementsBaseVertex(GLenum mode, const GLsizei *count, GLenum type, const void *const *indices, GLsizei drawcount, const GLint *basevertex) {
    GLint program;
    gles.core.glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    GLint draw_id_loc = gles.core.glGetUniformLocation(program, "glt_draw_id");

    if (draw_id_loc != -1) {
        for (GLsizei i = 0; i < drawcount; ++i) {
            if (count[i] > 0) {
                gles.core.glUniform1i(draw_id_loc, i);
                gles.core.glDrawElementsBaseVertex(mode, count[i], type, indices[i], basevertex[i]);
            }
        }
    } else {
        if(gles.ext.glMultiDrawElementsBaseVertexEXT) {
            gles.ext.glMultiDrawElementsBaseVertexEXT(mode, count, type, indices, drawcount, basevertex);
        } else {
            for (GLsizei i = 0; i < drawcount; ++i) {
                if (count[i] > 0) {
                    gles.core.glDrawElementsBaseVertex(mode, count[i], type, indices[i], basevertex[i]);
                }
            }
        }
    }
}

void glMultiDrawElementsIndirect(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride) {
    // If stride is 0, the commands are tightly packed.
    if (stride == 0) {
        stride = 20; // sizeof(count, instanceCount, firstIndex, baseVertex, baseInstance) -> 5 * sizeof(GLuint)
    }

    GLint program;
    gles.core.glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    GLint draw_id_loc = gles.core.glGetUniformLocation(program, "glt_draw_id");
    GLint base_instance_loc = gles.core.glGetUniformLocation(program, "glt_base_instance");

    if (draw_id_loc == -1 && base_instance_loc == -1) {
        if (gles.ext.glMultiDrawElementsIndirectEXT) {
            gles.ext.glMultiDrawElementsIndirectEXT(mode, type, indirect, drawcount, stride);
        } else {
            UNIMPLEMENTED();
        }
        return;
    }

    if (base_instance_loc != -1) {
        GLint indirect_buffer;
        gles.core.glGetIntegerv(GL_DRAW_INDIRECT_BUFFER_BINDING, &indirect_buffer);
        if (indirect_buffer == 0) return;

        typedef struct {
            GLuint count;
            GLuint instanceCount;
            GLuint firstIndex;
            GLuint baseVertex;
            GLuint baseInstance;
        } DrawElementsIndirectCommand;
        void* mapped_ptr = gles.core.glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, (GLsizeiptr)indirect, drawcount * stride, GL_MAP_READ_BIT);
        if (!mapped_ptr) return;

        for (GLsizei i = 0; i < drawcount; ++i) {
            DrawElementsIndirectCommand* cmd = (DrawElementsIndirectCommand*)((uint8_t*)mapped_ptr + i * stride);
            if (cmd->instanceCount > 0) {
                if (draw_id_loc != -1) gles.core.glUniform1i(draw_id_loc, i);
                gles.core.glUniform1i(base_instance_loc, cmd->baseInstance); // Set base instance from struct
                const void* indices_ptr = (const void*)(uintptr_t)(cmd->firstIndex * sizeof(GLuint));
                gles.core.glDrawElementsInstancedBaseVertex(mode, cmd->count, type, indices_ptr, cmd->instanceCount, cmd->baseVertex);
            }
        }
        gles.core.glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);

    } else {
        for (GLsizei i = 0; i < drawcount; ++i) {
            if (draw_id_loc != -1) {
                gles.core.glUniform1i(draw_id_loc, i);
            }
            const void* current_indirect_offset = (const void*)((uintptr_t)indirect + i * stride);
            gles.core.glDrawElementsIndirect(mode, type, current_indirect_offset);
        }
    }
}

void glMultiDrawElementsIndirectCount(GLenum mode, GLenum type, const void *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glMultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glNamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    gles.core.glBufferData(target, size, data, usage);
    gles.core.glBindBuffer(target, old_buffer);
}

void glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    if(gles.ext.glBufferStorageEXT) {
        gles.ext.glBufferStorageEXT(target, size, data, flags);
    }
    else {
        gles.core.glBufferData(target, size, data, GL_STATIC_DRAW);
    }
    gles.core.glBindBuffer(target, old_buffer);
}

void glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    gles.core.glBufferSubData(target, offset, size, data);
    gles.core.glBindBuffer(target, old_buffer);
}

void glNamedFramebufferDrawBuffer(GLuint framebuffer, GLenum buf) {
    UNIMPLEMENTED();
}

void glNamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum *bufs) {
    UNIMPLEMENTED();
}

void glNamedFramebufferParameteri(GLuint framebuffer, GLenum pname, GLint param) {
    UNIMPLEMENTED();
}

void glNamedFramebufferReadBuffer(GLuint framebuffer, GLenum src) {
    UNIMPLEMENTED();
}

void glNamedFramebufferRenderbuffer(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {
    const GLenum target = GL_FRAMEBUFFER;
    GLint old_fbo;
    gles.core.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
    gles.core.glBindFramebuffer(target, framebuffer);
    gles.core.glFramebufferRenderbuffer(framebuffer, attachment, renderbuffertarget, renderbuffer);
    gles.core.glBindFramebuffer(target, old_fbo);
}

void glNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level) {
    const GLenum target = GL_FRAMEBUFFER;
    GLint old_fbo;
    gles.core.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
    gles.core.glBindFramebuffer(target, framebuffer);
    gles.core.glFramebufferTexture(target, attachment, texture, level);
    gles.core.glBindFramebuffer(target, old_fbo);
}

void glNamedFramebufferTextureLayer(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer) {
    UNIMPLEMENTED();
}

void glNamedRenderbufferStorage(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height) {
    UNIMPLEMENTED();
}

void glNamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
    UNIMPLEMENTED();
}

void glNormalP3ui(GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glNormalP3uiv(GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glObjectLabel(GLenum identifier, GLuint name, GLsizei length, const GLchar *label) {
    gles.core.glObjectLabel(identifier, name, length, label);
}

void glObjectPtrLabel(const void *ptr, GLsizei length, const GLchar *label) {
    gles.core.glObjectPtrLabel(ptr, length, label);
}

void glPatchParameterfv(GLenum pname, const GLfloat *values) {
    UNIMPLEMENTED();
}

void glPatchParameteri(GLenum pname, GLint value) {
    gles.core.glPatchParameteri(pname, value);
}

void glPauseTransformFeedback(void) {
    gles.core.glPauseTransformFeedback();
}

void glPixelStoref(GLenum pname, GLfloat param) {
    UNIMPLEMENTED();
}

void glPixelStorei(GLenum pname, GLint param) {
    gles.core.glPixelStorei(pname, param);
}

void glPointParameterf(GLenum pname, GLfloat param) {
    UNIMPLEMENTED();
}

void glPointParameterfv(GLenum pname, const GLfloat *params) {
    UNIMPLEMENTED();
}

void glPointParameteri(GLenum pname, GLint param) {
    UNIMPLEMENTED();
}

void glPointParameteriv(GLenum pname, const GLint *params) {
    UNIMPLEMENTED();
}

void glPointSize(GLfloat size) {
    UNIMPLEMENTED();
}

void glPolygonMode(GLenum face, GLenum mode) {
    if(gles.ext.glPolygonModeNV) gles.ext.glPolygonModeNV(face, mode);
    else UNIMPLEMENTED();
}

void glPolygonOffset(GLfloat factor, GLfloat units) {
    gles.core.glPolygonOffset(factor, units);
}

void glPolygonOffsetClamp(GLfloat factor, GLfloat units, GLfloat clamp) {
    if(gles.ext.glPolygonOffsetClampEXT) gles.ext.glPolygonOffsetClampEXT(factor, units, clamp);
    else UNIMPLEMENTED();
}

void glPopDebugGroup(void) {
    gles.core.glPopDebugGroup();
}

void glPrimitiveRestartIndex(GLuint index) {
    UNIMPLEMENTED();
}

void glProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length) {
    gles.core.glProgramBinary(program, binaryFormat, binary, length);
}

void glProgramParameteri(GLuint program, GLenum pname, GLint value) {
    gles.core.glProgramParameteri(program, pname, value);
}

void glProgramUniform1d(GLuint program, GLint location, GLdouble v0) {
    UNIMPLEMENTED();
}

void glProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniform1f(GLuint program, GLint location, GLfloat v0) {
    gles.core.glProgramUniform1f(program, location, v0);
}

void glProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glProgramUniform1fv(program, location, count, value);
}

void glProgramUniform1i(GLuint program, GLint location, GLint v0) {
    gles.core.glProgramUniform1i(program, location, v0);
}

void glProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint *value) {
    gles.core.glProgramUniform1iv(program, location, count, value);
}

void glProgramUniform1ui(GLuint program, GLint location, GLuint v0) {
    gles.core.glProgramUniform1ui(program, location, v0);
}

void glProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint *value) {
    gles.core.glProgramUniform1uiv(program, location, count, value);
}

void glProgramUniform2d(GLuint program, GLint location, GLdouble v0, GLdouble v1) {
    UNIMPLEMENTED();
}

void glProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1) {
    gles.core.glProgramUniform2f(program, location, v0, v1);
}

void glProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glProgramUniform2fv(program, location, count, value);
}

void glProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1) {
    gles.core.glProgramUniform2i(program, location, v0, v1);
}

void glProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint *value) {
    gles.core.glProgramUniform2iv(program, location, count, value);
}

void glProgramUniform2ui(GLuint program, GLint location, GLuint v0, GLuint v1) {
    gles.core.glProgramUniform2ui(program, location, v0, v1);
}

void glProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint *value) {
    gles.core.glProgramUniform2uiv(program, location, count, value);
}

void glProgramUniform3d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2) {
    UNIMPLEMENTED();
}

void glProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    gles.core.glProgramUniform3f(program, location, v0, v1, v2);
}

void glProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glProgramUniform3fv(program, location, count, value);
}

void glProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2) {
    gles.core.glProgramUniform3i(program, location, v0, v1, v2);
}

void glProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint *value) {
    gles.core.glProgramUniform3iv(program, location, count, value);
}

void glProgramUniform3ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2) {
    gles.core.glProgramUniform3ui(program, location, v0, v1, v2);
}

void glProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint *value) {
    gles.core.glProgramUniform3uiv(program, location, count, value);
}

void glProgramUniform4d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3) {
    UNIMPLEMENTED();
}

void glProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    gles.core.glProgramUniform4f(program, location, v0, v1, v2, v3);
}

void glProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glProgramUniform4fv(program, location, count, value);
}

void glProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
    gles.core.glProgramUniform4i(program, location, v0, v1, v2, v3);
}

void glProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint *value) {
    gles.core.glProgramUniform4iv(program, location, count, value);
}

void glProgramUniform4ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    gles.core.glProgramUniform4ui(program, location, v0, v1, v2, v3);
}

void glProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint *value) {
    gles.core.glProgramUniform4uiv(program, location, count, value);
}

void glProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix2fv(program, location, count, transpose, value);
}

void glProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix2x3fv(program, location, count, transpose, value);
}

void glProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix2x4fv(program, location, count, transpose, value);
    UNIMPLEMENTED();
}

void glProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix3fv(program, location, count, transpose, value);
    UNIMPLEMENTED();
}

void glProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix3x2fv(program, location, count, transpose, value);
    UNIMPLEMENTED();
}

void glProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix3x4fv(program, location, count, transpose, value);
    UNIMPLEMENTED();
}

void glProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix4fv(program, location, count, transpose, value);
    UNIMPLEMENTED();
}

void glProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix4x2fv(program, location, count, transpose, value);
    UNIMPLEMENTED();
}

void glProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glProgramUniformMatrix4x3fv(program, location, count, transpose, value);
    UNIMPLEMENTED();
}

void glProvokingVertex(GLenum mode) {
    UNIMPLEMENTED();
}

void glPushDebugGroup(GLenum source, GLuint id, GLsizei length, const GLchar *message) {
    gles.core.glPushDebugGroup(source, id, length, message);
}

void glQueryCounter(GLuint id, GLenum target) {
    if(gles.ext.glQueryCounterEXT) gles.ext.glQueryCounterEXT(id, target);
    else UNIMPLEMENTED();
}

void glReadBuffer(GLenum src) {
    gles.core.glReadBuffer(src);
}

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) {
    GLint pbo = 0;
    gles.core.glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &pbo);

    if (pbo != 0) {
        GLint is_mapped = GL_FALSE;
        gles.core.glGetBufferParameteriv(GL_PIXEL_PACK_BUFFER, GL_BUFFER_MAPPED, &is_mapped);
        if (is_mapped) {
            void* mapped_ptr = NULL;
            gles.core.glGetBufferPointerv(GL_PIXEL_PACK_BUFFER, GL_BUFFER_MAP_POINTER, &mapped_ptr);
            GLint pbo_size = 0;
            gles.core.glGetBufferParameteriv(GL_PIXEL_PACK_BUFFER, GL_BUFFER_SIZE, &pbo_size);
            GLintptr offset = (GLintptr)pixels;
            GLsizei max_read_size = pbo_size - offset;
            void* temp_buffer = malloc(max_read_size);
            gles.core.glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            gles.core.glReadnPixels(x, y, width, height, format, type, max_read_size, temp_buffer);
            gles.core.glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
            memcpy((char*)mapped_ptr + offset, temp_buffer, max_read_size);
            free(temp_buffer);
            return;
        }
    }

    // Default case: No PBO is bound, or it's not mapped.
    gles.core.glReadPixels(x, y, width, height, format, type, pixels);
}

void glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data) {
    GLint pbo = 0;
    gles.core.glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &pbo);

    if (pbo != 0) {
        GLint is_mapped = GL_FALSE;
        gles.core.glGetBufferParameteriv(GL_PIXEL_PACK_BUFFER, GL_BUFFER_MAPPED, &is_mapped);

        if (is_mapped) {
            void* mapped_ptr = NULL;
            gles.core.glGetBufferPointerv(GL_PIXEL_PACK_BUFFER, GL_BUFFER_MAP_POINTER, &mapped_ptr);
            if (!mapped_ptr) {
                gles.core.glReadnPixels(x, y, width, height, format, type, bufSize, data);
                return;
            }

            // This implementation was already correct, as it has bufSize.
            void* temp_buffer = malloc(bufSize);
            if (!temp_buffer) {
                fprintf(stderr, "glReadnPixels workaround failed: could not allocate temporary buffer.\n");
                gles.core.glReadnPixels(x, y, width, height, format, type, bufSize, data);
                return;
            }

            gles.core.glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            gles.core.glReadnPixels(x, y, width, height, format, type, bufSize, temp_buffer);
            gles.core.glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);

            GLintptr offset = (GLintptr)data;
            memcpy((char*)mapped_ptr + offset, temp_buffer, bufSize);

            free(temp_buffer);
            return;
        }
    }

    gles.core.glReadnPixels(x, y, width, height, format, type, bufSize, data);
}

void glReleaseShaderCompiler(void) {
    gles.core.glReleaseShaderCompiler();
}

void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {
    gles.core.glRenderbufferStorage(target, internalformat, width, height);
}

void glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) {
    gles.core.glRenderbufferStorageMultisample(target, samples, internalformat, width, height);
}

void glResumeTransformFeedback(void) {
    gles.core.glResumeTransformFeedback();
}

void glSampleCoverage(GLfloat value, GLboolean invert) {
    gles.core.glSampleCoverage(value, invert);
}

void glSampleMaski(GLuint maskNumber, GLbitfield mask) {
    gles.core.glSampleMaski(maskNumber, mask);
}

void glSamplerParameterIiv(GLuint sampler, GLenum pname, const GLint *param) {
    gles.core.glSamplerParameterIiv(sampler, pname, param);
}

void glSamplerParameterIuiv(GLuint sampler, GLenum pname, const GLuint *param) {
    gles.core.glSamplerParameterIuiv(sampler, pname, param);
}

void glSamplerParameterf(GLuint sampler, GLenum pname, GLfloat param) {
    gles.core.glSamplerParameterf(sampler, pname, param);
}

void glSamplerParameterfv(GLuint sampler, GLenum pname, const GLfloat *param) {
    gles.core.glSamplerParameterfv(sampler, pname, param);
}

void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param) {
    gles.core.glSamplerParameteri(sampler, pname, param);
}

void glSamplerParameteriv(GLuint sampler, GLenum pname, const GLint *param) {
    gles.core.glSamplerParameteriv(sampler, pname, param);
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
    gles.core.glScissor(x, y, width, height);
}

void glScissorArrayv(GLuint first, GLsizei count, const GLint *v) {
    if(gles.ext.glScissorArrayvOES) gles.ext.glScissorArrayvOES(first, count, v);
    else UNIMPLEMENTED();
}

void glScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height) {
    if(gles.ext.glScissorIndexedOES) gles.ext.glScissorIndexedOES(index, left, bottom, width, height);
    else UNIMPLEMENTED();
}

void glScissorIndexedv(GLuint index, const GLint *v) {
    if(gles.ext.glScissorIndexedvOES) gles.ext.glScissorIndexedvOES(index, v);
    else UNIMPLEMENTED();
}

void glSecondaryColorP3ui(GLenum type, GLuint color) {
    UNIMPLEMENTED();
}

void glSecondaryColorP3uiv(GLenum type, const GLuint *color) {
    UNIMPLEMENTED();
}

void glShaderBinary(GLsizei count, const GLuint *shaders, GLenum binaryFormat, const void *binary, GLsizei length) {
    gles.core.glShaderBinary(count, shaders, binaryFormat, binary, length);
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) {
    if (count <= 0 || !string) {
        gles.core.glShaderSource(shader, count, string, length);
        return;
    }

    const char* original_source_to_process;
    char* concatenated_source_buffer = NULL;
    if (count > 1) {
        size_t total_len = 0;
        for (int i = 0; i < count; ++i) {
            if (string[i]) {
                total_len += (length && length[i] >= 0) ? length[i] : strlen(string[i]);
            }
        }

        concatenated_source_buffer = malloc(total_len + 1);
        if (!concatenated_source_buffer) {
            fprintf(stderr, "[Layer] Failed to allocate memory for source concatenation.\n");
            return;
        }
        concatenated_source_buffer[0] = '\0';

        char* current_pos = concatenated_source_buffer;
        for (int i = 0; i < count; ++i) {
            if (string[i]) {
                size_t chunk_len = (length && length[i] >= 0) ? length[i] : strlen(string[i]);
                memcpy(current_pos, string[i], chunk_len);
                current_pos += chunk_len;
            }
        }
        *current_pos = '\0';
        original_source_to_process = concatenated_source_buffer;
    } else {
        original_source_to_process = string[0];
    }

    shader_cache_add_source(shader, original_source_to_process);
    GLint shader_type;
    gles.core.glGetShaderiv(shader, GL_SHADER_TYPE, &shader_type);

    char* translated_source = shader_translate(shader_type, original_source_to_process);
    if (!translated_source) {
        fprintf(stderr, "[Layer] Shader translation failed!\n");
        if (concatenated_source_buffer) free(concatenated_source_buffer);
        return;
    }

    gles.core.glShaderSource(shader, 1, (const GLchar**)&translated_source, NULL);

    free(translated_source);
    if (concatenated_source_buffer) {
        free(concatenated_source_buffer);
    }
}

void glShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding) {
    UNIMPLEMENTED();
}

void glSpecializeShader(GLuint shader, const GLchar *pEntryPoint, GLuint numSpecializationConstants, const GLuint *pConstantIndex, const GLuint *pConstantValue) {
    UNIMPLEMENTED();
}

void glStencilFunc(GLenum func, GLint ref, GLuint mask) {
    gles.core.glStencilFunc(func, ref, mask);
}

void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
    gles.core.glStencilFuncSeparate(face, func, ref, mask);
}

void glStencilMask(GLuint mask) {
    gles.core.glStencilMask(mask);
}

void glStencilMaskSeparate(GLenum face, GLuint mask) {
    gles.core.glStencilMaskSeparate(face, mask);
}

void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
    gles.core.glStencilOp(fail, zfail, zpass);
}

void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
    gles.core.glStencilOpSeparate(face, sfail, dpfail, dppass);
}

void glTexBuffer(GLenum target, GLenum internalformat, GLuint buffer) {
    gles.core.glTexBuffer(target, internalformat, buffer);
}

void glTexBufferRange(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    gles.core.glTexBufferRange(target, internalformat, buffer, offset, size);
}

void glTexCoordP1ui(GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glTexCoordP1uiv(GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glTexCoordP2ui(GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glTexCoordP2uiv(GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glTexCoordP3ui(GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glTexCoordP3uiv(GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glTexCoordP4ui(GLenum type, GLuint coords) {
    UNIMPLEMENTED();
}

void glTexCoordP4uiv(GLenum type, const GLuint *coords) {
    UNIMPLEMENTED();
}

void glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels) {
    UNIMPLEMENTED();
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
    gles.core.glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    UNIMPLEMENTED();
}

void glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) {
    gles.core.glTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels);
}

void glTexImage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
    UNIMPLEMENTED();
}

void glTexParameterIiv(GLenum target, GLenum pname, const GLint *params) {
    gles.core.glTexParameterIiv(target, pname, params);
}

void glTexParameterIuiv(GLenum target, GLenum pname, const GLuint *params) {
    gles.core.glTexParameterIuiv(target, pname, params);
}

void glTexParameterf(GLenum target, GLenum pname, GLfloat param) {
    gles.core.glTexParameterf(target, pname, param);
}

void glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params) {
    gles.core.glTexParameterfv(target, pname, params);
}

void glTexParameteri(GLenum target, GLenum pname, GLint param) {
    gles.core.glTexParameteri(target, pname, param);
}

void glTexParameteriv(GLenum target, GLenum pname, const GLint *params) {
    gles.core.glTexParameteriv(target, pname, params);
}

void glTexStorage1D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) {
    UNIMPLEMENTED();
}

void glTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
    gles.core.glTexStorage2D(target, levels, internalformat, width, height);
}

void glTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    gles.core.glTexStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
}

void glTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
    gles.core.glTexStorage3D(target, levels, internalformat, width, height, depth);
}

void glTexStorage3DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
    gles.core.glTexStorage3DMultisample(target, samples, internalformat, width, height, depth, fixedsamplelocations);
}

void glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
    UNIMPLEMENTED();
}

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
    gles.core.glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
    gles.core.glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
}

void glTextureBarrier(void) {
    UNIMPLEMENTED();
}

void glTextureBuffer(GLuint texture, GLenum internalformat, GLuint buffer) {
    UNIMPLEMENTED();
}

void glTextureBufferRange(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    UNIMPLEMENTED();
}

void glTextureParameterIiv(GLuint texture, GLenum pname, const GLint *params) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexParameterIiv(target, pname, params);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureParameterIuiv(GLuint texture, GLenum pname, const GLuint *params) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexParameterIuiv(target, pname, params);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureParameterf(GLuint texture, GLenum pname, GLfloat param) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexParameterf(target, pname, param);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat *param) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexParameterfv(target, pname, param);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureParameteri(GLuint texture, GLenum pname, GLint param) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexParameteri(target, pname, param);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureParameteriv(GLuint texture, GLenum pname, const GLint *param) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexParameteriv(target, pname, param);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureStorage1D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width) {
    UNIMPLEMENTED();
}

void glTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexStorage2D(target, levels, internalformat, width, height);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) {
    UNIMPLEMENTED();
}

void glTextureStorage3D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexStorage3D(target, levels, internalformat, width, height, depth);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) {
    UNIMPLEMENTED();
}

void glTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels) {
    UNIMPLEMENTED();
}

void glTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
    GLenum target = state_texture_get_target(texture);

    GLenum binding_target = get_texture_binding_from_target(target);
    GLint old_texture;
    gles.core.glGetIntegerv(binding_target, &old_texture);

    gles.core.glBindTexture(target, texture);
    gles.core.glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    gles.core.glBindTexture(target, old_texture);
}

void glTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) {
    UNIMPLEMENTED();
}

void glTextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers) {
    if(gles.ext.glTextureViewEXT) gles.ext.glTextureViewEXT(texture, target, origtexture, internalformat, minlevel, numlevels, minlayer, numlayers);
    else UNIMPLEMENTED();
}

void glTransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer) {
    UNIMPLEMENTED();
}

void glTransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    UNIMPLEMENTED();
}

void glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar *const *varyings, GLenum bufferMode) {
    gles.core.glTransformFeedbackVaryings(program, count, varyings, bufferMode);
}

void glUniform1d(GLint location, GLdouble x) {
    UNIMPLEMENTED();
}

void glUniform1dv(GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniform1f(GLint location, GLfloat v0) {
    gles.core.glUniform1f(location, v0);
}

void glUniform1fv(GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glUniform1fv(location, count, value);
}

void glUniform1i(GLint location, GLint v0) {
    gles.core.glUniform1i(location, v0);
}

void glUniform1iv(GLint location, GLsizei count, const GLint *value) {
    gles.core.glUniform1iv(location, count, value);
}

void glUniform1ui(GLint location, GLuint v0) {
    gles.core.glUniform1ui(location, v0);
}

void glUniform1uiv(GLint location, GLsizei count, const GLuint *value) {
    gles.core.glUniform1uiv(location, count, value);
}

void glUniform2d(GLint location, GLdouble x, GLdouble y) {
    UNIMPLEMENTED();
}

void glUniform2dv(GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniform2f(GLint location, GLfloat v0, GLfloat v1) {
    gles.core.glUniform2f(location, v0, v1);
}

void glUniform2fv(GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glUniform2fv(location, count, value);
}

void glUniform2i(GLint location, GLint v0, GLint v1) {
    gles.core.glUniform2i(location, v0, v1);
}

void glUniform2iv(GLint location, GLsizei count, const GLint *value) {
    gles.core.glUniform2iv(location, count, value);
}

void glUniform2ui(GLint location, GLuint v0, GLuint v1) {
    gles.core.glUniform2ui(location, v0, v1);
}

void glUniform2uiv(GLint location, GLsizei count, const GLuint *value) {
    gles.core.glUniform2uiv(location, count, value);
}

void glUniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z) {
    UNIMPLEMENTED();
}

void glUniform3dv(GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    gles.core.glUniform3f(location, v0, v1, v2);
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glUniform3fv(location, count, value);
}

void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
    gles.core.glUniform3i(location, v0, v1, v2);
}

void glUniform3iv(GLint location, GLsizei count, const GLint *value) {
    gles.core.glUniform3iv(location, count, value);
}

void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2) {
    gles.core.glUniform3ui(location, v0, v1, v2);
}

void glUniform3uiv(GLint location, GLsizei count, const GLuint *value) {
    gles.core.glUniform3uiv(location, count, value);
}

void glUniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
    UNIMPLEMENTED();
}

void glUniform4dv(GLint location, GLsizei count, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    gles.core.glUniform4f(location, v0, v1, v2, v3);
}

void glUniform4fv(GLint location, GLsizei count, const GLfloat *value) {
    gles.core.glUniform4fv(location, count, value);
}

void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
    gles.core.glUniform4i(location, v0, v1, v2, v3);
}

void glUniform4iv(GLint location, GLsizei count, const GLint *value) {
    gles.core.glUniform4iv(location, count, value);
}

void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    gles.core.glUniform4ui(location, v0, v1, v2, v3);
}

void glUniform4uiv(GLint location, GLsizei count, const GLuint *value) {
    gles.core.glUniform4uiv(location, count, value);
}

void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) {
    gles.core.glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

void glUniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix2fv(location, count, transpose, value);
}

void glUniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix2x3fv(location, count, transpose, value);
}

void glUniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix2x4fv(location, count, transpose, value);
}

void glUniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix3fv(location, count, transpose, value);
}

void glUniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix3x2fv(location, count, transpose, value);
}

void glUniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix3x4fv(location, count, transpose, value);
}

void glUniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix4fv(location, count, transpose, value);
}

void glUniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix4x2fv(location, count, transpose, value);
}

void glUniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value) {
    UNIMPLEMENTED();
}

void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) {
    gles.core.glUniformMatrix4x3fv(location, count, transpose, value);
}

void glUniformSubroutinesuiv(GLenum shadertype, GLsizei count, const GLuint *indices) {
    UNIMPLEMENTED();
}

GLboolean glUnmapBuffer(GLenum target) {
    return gles.core.glUnmapBuffer(target);
}

GLboolean glUnmapNamedBuffer(GLuint buffer) {
    const GLenum target = GL_ARRAY_BUFFER;
    GLboolean result;

    GLint old_buffer;
    gles.core.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_buffer);
    gles.core.glBindBuffer(target, buffer);
    result = gles.core.glUnmapBuffer(target);
    gles.core.glBindBuffer(target, old_buffer);

    return result;
}

void glUseProgram(GLuint program) {
    gles.core.glUseProgram(program);
}

void glUseProgramStages(GLuint pipeline, GLbitfield stages, GLuint program) {
    gles.core.glUseProgramStages(pipeline, stages, program);
}

void glValidateProgram(GLuint program) {
    gles.core.glValidateProgram(program);
}

void glValidateProgramPipeline(GLuint pipeline) {
    gles.core.glValidateProgramPipeline(pipeline);
}

void glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex) {
    GLint old_vao;
    gles.core.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_vao);
    gles.core.glBindVertexArray(vaobj);
    gles.core.glVertexAttribBinding(attribindex, bindingindex);
    gles.core.glBindVertexArray(old_vao);
}

void glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
    GLint old_vao;
    gles.core.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_vao);
    gles.core.glBindVertexArray(vaobj);
    gles.core.glVertexAttribFormat(attribindex, size, type, normalized, relativeoffset);
    gles.core.glBindVertexArray(old_vao);
}

void glVertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    GLint old_vao;
    gles.core.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_vao);
    gles.core.glBindVertexArray(vaobj);
    gles.core.glVertexAttribIFormat(attribindex, size, type, relativeoffset);
    gles.core.glBindVertexArray(old_vao);
}

void glVertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    UNIMPLEMENTED();
}

void glVertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor) {
    UNIMPLEMENTED();
}

void glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer) {
    GLint old_vao;
    gles.core.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_vao);
    gles.core.glBindVertexArray(vaobj);
    gles.core.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    gles.core.glBindVertexArray(old_vao);
}

void glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride) {
    GLint old_vao;
    gles.core.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_vao);
    gles.core.glBindVertexArray(vaobj);
    gles.core.glBindVertexBuffer(bindingindex, buffer, offset, stride);
    gles.core.glBindVertexArray(old_vao);
}

void glVertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides) {
    UNIMPLEMENTED();
}

void glVertexAttrib1d(GLuint index, GLdouble x) {
    UNIMPLEMENTED();
}

void glVertexAttrib1dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib1f(GLuint index, GLfloat x) {
    gles.core.glVertexAttrib1f(index, x);
}

void glVertexAttrib1fv(GLuint index, const GLfloat *v) {
    gles.core.glVertexAttrib1fv(index, v);
}

void glVertexAttrib1s(GLuint index, GLshort x) {
    UNIMPLEMENTED();
}

void glVertexAttrib1sv(GLuint index, const GLshort *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib2d(GLuint index, GLdouble x, GLdouble y) {
    UNIMPLEMENTED();
}

void glVertexAttrib2dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y) {
    gles.core.glVertexAttrib2f(index, x, y);
}

void glVertexAttrib2fv(GLuint index, const GLfloat *v) {
    gles.core.glVertexAttrib2fv(index, v);
}

void glVertexAttrib2s(GLuint index, GLshort x, GLshort y) {
    UNIMPLEMENTED();
}

void glVertexAttrib2sv(GLuint index, const GLshort *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) {
    UNIMPLEMENTED();
}

void glVertexAttrib3dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z) {
    gles.core.glVertexAttrib3f(index, x, y, z);
}

void glVertexAttrib3fv(GLuint index, const GLfloat *v) {
    gles.core.glVertexAttrib3fv(index, v);
}

void glVertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z) {
    UNIMPLEMENTED();
}

void glVertexAttrib3sv(GLuint index, const GLshort *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4Nbv(GLuint index, const GLbyte *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4Niv(GLuint index, const GLint *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4Nsv(GLuint index, const GLshort *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) {
    UNIMPLEMENTED();
}

void glVertexAttrib4Nubv(GLuint index, const GLubyte *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4Nuiv(GLuint index, const GLuint *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4Nusv(GLuint index, const GLushort *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4bv(GLuint index, const GLbyte *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
    UNIMPLEMENTED();
}

void glVertexAttrib4dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    gles.core.glVertexAttrib4f(index, x, y, z, w);
}

void glVertexAttrib4fv(GLuint index, const GLfloat *v) {
    gles.core.glVertexAttrib4fv(index, v);
}

void glVertexAttrib4iv(GLuint index, const GLint *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) {
    UNIMPLEMENTED();
}

void glVertexAttrib4sv(GLuint index, const GLshort *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4ubv(GLuint index, const GLubyte *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4uiv(GLuint index, const GLuint *v) {
    UNIMPLEMENTED();
}

void glVertexAttrib4usv(GLuint index, const GLushort *v) {
    UNIMPLEMENTED();
}

void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex) {
    gles.core.glVertexAttribBinding(attribindex, bindingindex);
}

void glVertexAttribDivisor(GLuint index, GLuint divisor) {
    gles.core.glVertexAttribDivisor(index, divisor);
}

void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
    gles.core.glVertexAttribFormat(attribindex, size, type, normalized, relativeoffset);
}

void glVertexAttribI1i(GLuint index, GLint x) {
    UNIMPLEMENTED();
}

void glVertexAttribI1iv(GLuint index, const GLint *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI1ui(GLuint index, GLuint x) {
    UNIMPLEMENTED();
}

void glVertexAttribI1uiv(GLuint index, const GLuint *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI2i(GLuint index, GLint x, GLint y) {
    UNIMPLEMENTED();
}

void glVertexAttribI2iv(GLuint index, const GLint *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI2ui(GLuint index, GLuint x, GLuint y) {
    UNIMPLEMENTED();
}

void glVertexAttribI2uiv(GLuint index, const GLuint *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI3i(GLuint index, GLint x, GLint y, GLint z) {
    UNIMPLEMENTED();
}

void glVertexAttribI3iv(GLuint index, const GLint *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z) {
    UNIMPLEMENTED();
}

void glVertexAttribI3uiv(GLuint index, const GLuint *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI4bv(GLuint index, const GLbyte *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w) {
    gles.core.glVertexAttribI4i(index, x, y, z, w);
}

void glVertexAttribI4iv(GLuint index, const GLint *v) {
    gles.core.glVertexAttribI4iv(index, v);
}

void glVertexAttribI4sv(GLuint index, const GLshort *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI4ubv(GLuint index, const GLubyte *v) {
    UNIMPLEMENTED();
}

void glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) {
    gles.core.glVertexAttribI4ui(index, x, y, z, w);
}

void glVertexAttribI4uiv(GLuint index, const GLuint *v) {
    gles.core.glVertexAttribI4uiv(index, v);
}

void glVertexAttribI4usv(GLuint index, const GLushort *v) {
    UNIMPLEMENTED();
}

void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    gles.core.glVertexAttribIFormat(attribindex, size, type, relativeoffset);
}

void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
    gles.core.glVertexAttribIPointer(index, size, type, stride, pointer);
}

void glVertexAttribL1d(GLuint index, GLdouble x) {
    UNIMPLEMENTED();
}

void glVertexAttribL1dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttribL2d(GLuint index, GLdouble x, GLdouble y) {
    UNIMPLEMENTED();
}

void glVertexAttribL2dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z) {
    UNIMPLEMENTED();
}

void glVertexAttribL3dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
    UNIMPLEMENTED();
}

void glVertexAttribL4dv(GLuint index, const GLdouble *v) {
    UNIMPLEMENTED();
}

void glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset) {
    UNIMPLEMENTED();
}

void glVertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer) {
    UNIMPLEMENTED();
}

void glVertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    UNIMPLEMENTED();
}

void glVertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    UNIMPLEMENTED();
}

void glVertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    UNIMPLEMENTED();
}

void glVertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    UNIMPLEMENTED();
}

void glVertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    UNIMPLEMENTED();
}

void glVertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    UNIMPLEMENTED();
}

void glVertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value) {
    UNIMPLEMENTED();
}

void glVertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value) {
    UNIMPLEMENTED();
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
    gles.core.glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void glVertexBindingDivisor(GLuint bindingindex, GLuint divisor) {
    gles.core.glVertexBindingDivisor(bindingindex, divisor);
}

void glVertexP2ui(GLenum type, GLuint value) {
    UNIMPLEMENTED();
}

void glVertexP2uiv(GLenum type, const GLuint *value) {
    UNIMPLEMENTED();
}

void glVertexP3ui(GLenum type, GLuint value) {
    UNIMPLEMENTED();
}

void glVertexP3uiv(GLenum type, const GLuint *value) {
    UNIMPLEMENTED();
}

void glVertexP4ui(GLenum type, GLuint value) {
    UNIMPLEMENTED();
}

void glVertexP4uiv(GLenum type, const GLuint *value) {
    UNIMPLEMENTED();
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    gles.core.glViewport(x, y, width, height);
}

void glViewportArrayv(GLuint first, GLsizei count, const GLfloat *v) {
    UNIMPLEMENTED();
}

void glViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h) {
    UNIMPLEMENTED();
}

void glViewportIndexedfv(GLuint index, const GLfloat *v) {
    UNIMPLEMENTED();
}

void glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout) {
    gles.core.glWaitSync(sync, flags, timeout);
}

#ifdef __cplusplus
}
#endif

