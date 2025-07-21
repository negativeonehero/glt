#include <GLES3/gl32.h>

#include "translate.h"

char* replace_between(const char* str, const char* start_str, const char* end_str, const char* replace_str) {
    // Find the start position
    const char* start_pos = strstr(str, start_str);
    if (start_pos == NULL) {
        // start_str not found, return a copy of original string
        return strdup(str);
    }

    // Find the end position after the start position
    const char* end_pos = strstr(start_pos + strlen(start_str), end_str);
    if (end_pos == NULL) {
        // end_str not found after start_str, return a copy of original string
        return strdup(str);
    }

    // Calculate lengths
    size_t prefix_len = start_pos - str;
    size_t suffix_len = strlen(end_pos + strlen(end_str));
    size_t replace_len = strlen(replace_str);

    // Allocate memory for the new string
    char* result = malloc(prefix_len + replace_len + suffix_len + 1);
    if (result == NULL) {
        return NULL; // memory allocation failed
    }

    // Build the new string
    memcpy(result, str, prefix_len); // copy prefix
    memcpy(result + prefix_len, replace_str, replace_len); // copy replacement
    strcpy(result + prefix_len + replace_len, end_pos + strlen(end_str)); // copy suffix

    return result;
}

char* insert_after_substring(const char* str, const char* find_str, int index, const char* insert_str) {
    // Handle invalid index
    if (index < 0) {
        return strdup(str);
    }

    const char* current_pos = str;
    const char* found_pos = NULL;
    int current_index = 0;
    size_t find_len = strlen(find_str);

    // Find the index-th occurrence of find_str
    while (current_index <= index) {
        found_pos = strstr(current_pos, find_str);
        if (found_pos == NULL) {
            // Not enough occurrences found, return copy of original
            return strdup(str);
        }
        current_pos = found_pos + find_len;
        current_index++;
    }

    // Calculate positions and lengths
    size_t prefix_len = found_pos - str + find_len;
    size_t suffix_len = strlen(found_pos + find_len);
    size_t insert_len = strlen(insert_str);
    size_t original_len = strlen(str);

    // Allocate memory for new string
    char* result = malloc(original_len + insert_len + 1);
    if (result == NULL) {
        return NULL; // memory allocation failed
    }

    // Build the new string
    memcpy(result, str, prefix_len); // copy prefix including the found substring
    memcpy(result + prefix_len, insert_str, insert_len); // insert the new string
    strcpy(result + prefix_len + insert_len, found_pos + find_len); // copy suffix

    return result;
}

char* shader_translate(GLenum shader_type, const char* original_source) {
    // Version directive
    char* translated = replace_between(original_source, "#version", "\n", "#version 320 es\n");
    if(shader_type == GL_FRAGMENT_SHADER)
        translated = insert_after_substring(translated, "\n", 0, "\nprecision mediump float;\nprecision mediump int;\n");
    return translated;
}
