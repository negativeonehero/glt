#include "translate.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // For malloc and free
#include <regex>

// We need to include the C++ headers for glslang and SPIRV-Cross
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>

// Include the SPIRV-Cross GLSL compiler
#include <spirv_cross/spirv_glsl.hpp>

// A helper to get the glslang shader stage from a GLenum
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
    static TBuiltInResource resources = {}; // Use static to initialize only once

    // Initialize with safe, common-sense defaults for desktop OpenGL.
    // Uninitialized fields will be 0.
    resources.maxVertexAttribs = 64;
    resources.maxVertexUniformComponents = 4096;
    resources.maxVaryingComponents = 60;
    resources.maxVertexOutputComponents = 64;
    resources.maxFragmentInputComponents = 128;
    resources.maxVertexTextureImageUnits = 32;
    resources.maxCombinedTextureImageUnits = 80;
    resources.maxTextureImageUnits = 32;
    resources.maxFragmentUniformComponents = 4096;
    resources.maxDrawBuffers = 8;
    resources.maxClipDistances = 8;
    resources.maxSamples = 4;
    resources.maxVaryingFloats = 64; // Deprecated but let's be safe

    // Set limits to allow most shader features
    resources.limits.nonInductiveForLoops = 1;
    resources.limits.whileLoops = 1;
    resources.limits.doWhileLoops = 1;
    resources.limits.generalUniformIndexing = 1;
    resources.limits.generalAttributeMatrixVectorIndexing = 1;
    resources.limits.generalVaryingIndexing = 1;
    resources.limits.generalSamplerIndexing = 1;
    resources.limits.generalVariableIndexing = 1;
    resources.limits.generalConstantMatrixVectorIndexing = 1;

    return resources;
}

// This is our C-callable function
extern "C" char* shader_translate(GLenum shader_type, const char* source) {

    // 1. Prepare the source string by replacing/prepending the #version directive.
    std::string source_str(source);
    std::string processed_source;

    // Find the start of a #version directive
    size_t version_pos = source_str.find("#version");
    // Find the first non-whitespace character
    size_t first_char_pos = source_str.find_first_not_of(" \t\r\n");
    if (first_char_pos == std::string::npos) first_char_pos = 0;

    // Check if #version exists and is at the beginning of the file.
    if (version_pos != std::string::npos && version_pos == first_char_pos) {
        // Found it. Find the end of that line.
        size_t eol_pos = source_str.find('\n', version_pos);
        if (eol_pos == std::string::npos) {
            // The #version is the only thing in the shader, so we just replace it.
            processed_source = "#version 450 compatibility\n";
        } else {
            // Take the substring *after* the #version line and prepend our new version.
            processed_source = "#version 450 compatibility\n" + source_str.substr(eol_pos + 1);
        }
    } else {
        // No #version directive found at the start, so prepend ours.
        processed_source = "#version 450 compatibility\n" + source_str;
    }

    // Initialize glslang
    glslang::InitializeProcess();

    EShLanguage stage = GetLanguage(shader_type);
    glslang::TShader shader(stage);
    
    // Use our *processed* source string
    const char* shaderStrings[1];
    shaderStrings[0] = processed_source.c_str();
    shader.setStrings(shaderStrings, 1);

    // This is still essential.
    shader.setAutoMapBindings(true);
    shader.setAutoMapLocations(true);

    glslang::EShClient Client = glslang::EShClientOpenGL;
    int ClientVersion = 450;
    shader.setEnvInput(glslang::EShSourceGlsl, stage, Client, ClientVersion);
    shader.setEnvClient(Client, (glslang::EShTargetClientVersion)ClientVersion);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);

    const TBuiltInResource& resources = GetDefaultBuiltInResources();
    EShMessages messages = EShMsgDefault;

    if (!shader.parse(&resources, 450, true, messages)) {
        // IMPORTANT: Log the PROCESSED source now so we can see what glslang actually saw.
        std::cerr << "GLSL Parsing Failed on processed source:\n" << processed_source << "\n";
        std::cerr << "Original source:\n" << source << "\n";
        std::cerr << shader.getInfoLog() << "\n" << shader.getInfoDebugLog() << std::endl;
        glslang::FinalizeProcess();
        // Fallback to returning original source
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

    uint32_t binding_counter = 0;
    spirv_cross::ShaderResources shader_resources = glsl.get_shader_resources();

    spirv_cross::CompilerGLSL::Options options;
    options.version = 320;
    options.es = true;
    options.force_zero_initialized_variables = true;
    glsl.set_common_options(options);
    
    std::string glsl_source = glsl.compile();
    
    glslang::FinalizeProcess();
    
    char* result = (char*)malloc(glsl_source.length() + 1);
    strcpy(result, glsl_source.c_str());

    return result;
}
