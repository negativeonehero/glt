#include "translate.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <regex>

#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>

#include <spirv_cross/spirv_glsl.hpp>

EShLanguage GetLanguage(GLenum shader_type) {
    switch (shader_type) {
        case GL_VERTEX_SHADER:          return EShLangVertex;
        case GL_FRAGMENT_SHADER:        return EShLangFragment;
        case GL_GEOMETRY_SHADER:        return EShLangGeometry;
        case GL_TESS_CONTROL_SHADER:    return EShLangTessControl;
        case GL_TESS_EVALUATION_SHADER: return EShLangTessEvaluation;
        case GL_COMPUTE_SHADER:         return EShLangCompute;
        default:                        return EShLangVertex;
    }
}

const TBuiltInResource GetDefaultBuiltInResources() {
    static TBuiltInResource Resources = {};

    Resources.maxLights                                 = 32;
    Resources.maxClipPlanes                             = 6;
    Resources.maxTextureUnits                           = 32;
    Resources.maxTextureCoords                          = 32;
    Resources.maxVertexAttribs                          = 64;
    Resources.maxVertexUniformComponents                = 4096;
    Resources.maxVaryingFloats                          = 64;
    Resources.maxVertexTextureImageUnits                = 32;
    Resources.maxCombinedTextureImageUnits              = 80;
    Resources.maxTextureImageUnits                      = 32;
    Resources.maxFragmentUniformComponents              = 4096;
    Resources.maxDrawBuffers                            = 32;
    Resources.maxVertexUniformVectors                   = 128;
    Resources.maxVaryingVectors                         = 8;
    Resources.maxFragmentUniformVectors                 = 16;
    Resources.maxVertexOutputVectors                    = 16;
    Resources.maxFragmentInputVectors                   = 15;
    Resources.minProgramTexelOffset                     = -8;
    Resources.maxProgramTexelOffset                     = 7;
    Resources.maxClipDistances                          = 8;
    Resources.maxComputeWorkGroupCountX                 = 65535;
    Resources.maxComputeWorkGroupCountY                 = 65535;
    Resources.maxComputeWorkGroupCountZ                 = 65535;
    Resources.maxComputeWorkGroupSizeX                  = 1024;
    Resources.maxComputeWorkGroupSizeY                  = 1024;
    Resources.maxComputeWorkGroupSizeZ                  = 64;
    Resources.maxComputeUniformComponents               = 1024;
    Resources.maxComputeTextureImageUnits               = 16;
    Resources.maxComputeImageUniforms                   = 8;
    Resources.maxComputeAtomicCounters                  = 8;
    Resources.maxComputeAtomicCounterBuffers            = 1;
    Resources.maxVaryingComponents                      = 60;
    Resources.maxVertexOutputComponents                 = 64;
    Resources.maxGeometryInputComponents                = 64;
    Resources.maxGeometryOutputComponents               = 128;
    Resources.maxFragmentInputComponents                = 128;
    Resources.maxImageUnits                             = 8;
    Resources.maxCombinedImageUnitsAndFragmentOutputs   = 8;
    Resources.maxCombinedShaderOutputResources          = 8;
    Resources.maxImageSamples                           = 0;
    Resources.maxVertexImageUniforms                    = 0;
    Resources.maxTessControlImageUniforms               = 0;
    Resources.maxTessEvaluationImageUniforms            = 0;
    Resources.maxGeometryImageUniforms                  = 0;
    Resources.maxFragmentImageUniforms                  = 8;
    Resources.maxCombinedImageUniforms                  = 8;
    Resources.maxGeometryTextureImageUnits              = 16;
    Resources.maxGeometryOutputVertices                 = 256;
    Resources.maxGeometryTotalOutputComponents          = 1024;
    Resources.maxGeometryUniformComponents              = 1024;
    Resources.maxGeometryVaryingComponents              = 64;
    Resources.maxTessControlInputComponents             = 128;
    Resources.maxTessControlOutputComponents            = 128;
    Resources.maxTessControlTextureImageUnits           = 16;
    Resources.maxTessControlUniformComponents           = 1024;
    Resources.maxTessControlTotalOutputComponents       = 4096;
    Resources.maxTessEvaluationInputComponents          = 128;
    Resources.maxTessEvaluationOutputComponents         = 128;
    Resources.maxTessEvaluationTextureImageUnits        = 16;
    Resources.maxTessEvaluationUniformComponents        = 1024;
    Resources.maxTessPatchComponents                    = 120;
    Resources.maxPatchVertices                          = 32;
    Resources.maxTessGenLevel                           = 64;
    Resources.maxViewports                              = 16;
    Resources.maxVertexAtomicCounters                   = 0;
    Resources.maxTessControlAtomicCounters              = 0;
    Resources.maxTessEvaluationAtomicCounters           = 0;
    Resources.maxGeometryAtomicCounters                 = 0;
    Resources.maxFragmentAtomicCounters                 = 8;
    Resources.maxCombinedAtomicCounters                 = 8;
    Resources.maxAtomicCounterBindings                  = 1;
    Resources.maxVertexAtomicCounterBuffers             = 0;
    Resources.maxTessControlAtomicCounterBuffers        = 0;
    Resources.maxTessEvaluationAtomicCounterBuffers     = 0;
    Resources.maxGeometryAtomicCounterBuffers           = 0;
    Resources.maxFragmentAtomicCounterBuffers           = 1;
    Resources.maxCombinedAtomicCounterBuffers           = 1;
    Resources.maxAtomicCounterBufferSize                = 16384;
    Resources.maxTransformFeedbackBuffers               = 4;
    Resources.maxTransformFeedbackInterleavedComponents = 64;
    Resources.maxCullDistances                          = 8;
    Resources.maxCombinedClipAndCullDistances           = 8;
    Resources.maxSamples                                = 4;
    Resources.maxMeshOutputVerticesNV                   = 256;
    Resources.maxMeshOutputPrimitivesNV                 = 512;
    Resources.maxMeshWorkGroupSizeX_NV                  = 32;
    Resources.maxMeshWorkGroupSizeY_NV                  = 1;
    Resources.maxMeshWorkGroupSizeZ_NV                  = 1;
    Resources.maxTaskWorkGroupSizeX_NV                  = 32;
    Resources.maxTaskWorkGroupSizeY_NV                  = 1;
    Resources.maxTaskWorkGroupSizeZ_NV                  = 1;
    Resources.maxMeshViewCountNV                        = 4;

    Resources.limits.nonInductiveForLoops                 = true;
    Resources.limits.whileLoops                           = true;
    Resources.limits.doWhileLoops                         = true;
    Resources.limits.generalUniformIndexing               = true;
    Resources.limits.generalAttributeMatrixVectorIndexing = true;
    Resources.limits.generalVaryingIndexing               = true;
    Resources.limits.generalSamplerIndexing               = true;
    Resources.limits.generalVariableIndexing              = true;
    Resources.limits.generalConstantMatrixVectorIndexing  = true;

    return Resources;
}

extern "C" char* shader_translate(GLenum shader_type, const char* source) {
    std::string source_str(source);
    std::string processed_source;

    auto preprocess_builtin = [&](const std::string& builtin_name, const std::string& uniform_name) {
        std::regex builtin_regex("\\b" + builtin_name + "\\b");
        if (std::regex_search(source_str, builtin_regex)) {
            source_str = std::regex_replace(source_str, builtin_regex, uniform_name);

            size_t version_end_pos = source_str.find('\n');
            if (version_end_pos == std::string::npos) {
                source_str += "\nuniform int " + uniform_name + ";\n";
            } else {
                source_str.insert(version_end_pos + 1, "uniform int " + uniform_name + ";\n");
            }
        }
    };

    preprocess_builtin("gl_DrawID", "glt_draw_id");
    preprocess_builtin("gl_BaseInstance", "glt_base_instance");

    size_t version_pos = source_str.find("#version");
    size_t first_char_pos = source_str.find_first_not_of(" \t\r\n");
    if (first_char_pos == std::string::npos) first_char_pos = 0;

    if (version_pos != std::string::npos && version_pos == first_char_pos) {
        size_t eol_pos = source_str.find('\n', version_pos);
        if (eol_pos == std::string::npos) {
            processed_source = "#version 460\n";
        } else {
            processed_source = "#version 460\n" + source_str.substr(eol_pos + 1);
        }
    } else {
        processed_source = "#version 460\n" + source_str;
    }



    glslang::InitializeProcess();

    EShLanguage stage = GetLanguage(shader_type);
    glslang::TShader shader(stage);

    const char* shaderStrings[1];
    shaderStrings[0] = processed_source.c_str();
    shader.setStrings(shaderStrings, 1);

    shader.setAutoMapBindings(true);
    shader.setAutoMapLocations(true);

    glslang::EShClient Client = glslang::EShClientOpenGL;
    int ClientVersion = 460;
    shader.setEnvInput(glslang::EShSourceGlsl, stage, Client, ClientVersion);
    shader.setEnvClient(Client, (glslang::EShTargetClientVersion)ClientVersion);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

    const TBuiltInResource& resources = GetDefaultBuiltInResources();
    EShMessages messages = EShMsgDefault;

    if (!shader.parse(&resources, 460, false, messages)) {
        std::cerr << "GLSL Parsing Failed on processed source:\n" << processed_source << "\n";
        std::cerr << "Original source:\n" << source << "\n";
        std::cerr << shader.getInfoLog() << "\n" << shader.getInfoDebugLog() << std::endl;
        glslang::FinalizeProcess();
        char* result = (char*)malloc(strlen(source) + 1);
        strcpy(result, source);
        return result;
    }

    glslang::TProgram program;
    program.addShader(&shader);

    if (!program.link(messages)) {
        std::cerr << "GLSL Linking Failed:\n" << processed_source << "\n";
        std::cerr << program.getInfoLog() << "\n" << program.getInfoDebugLog() << std::endl;
        glslang::FinalizeProcess();
        char* result = (char*)malloc(strlen(source) + 1);
        strcpy(result, source);
        return result;
    }

    std::vector<unsigned int> spirv;
    spv::SpvBuildLogger logger;
    glslang::SpvOptions spvOptions;
    glslang::GlslangToSpv(*program.getIntermediate(stage), spirv, &logger, &spvOptions);

    spirv_cross::CompilerGLSL glsl(std::move(spirv));

    spirv_cross::CompilerGLSL::Options options;
    options.version = 320;
    options.es = true;
    options.force_zero_initialized_variables = true;
    glsl.set_common_options(options);

    spirv_cross::ShaderResources shader_resources = glsl.get_shader_resources();

    std::string glsl_source = glsl.compile();
    glslang::FinalizeProcess();
    char* result = (char*)malloc(glsl_source.length() + 1);
    strcpy(result, glsl_source.c_str());

    return result;
}
