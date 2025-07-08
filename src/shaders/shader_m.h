#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <glad/glad.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <cglm/cglm.h>


// loads the shader then copies the data into a char variable and returns the shader id
int load_shader_file(const char* shader_file_name, int shader_type)
{
    FILE* shader_file = fopen(shader_file_name, "rb");
    if(shader_file != NULL)
    {
        char* shader_char;
        fseek(shader_file, 0, SEEK_END);
        size_t total_size = ftell(shader_file);
        rewind(shader_file);

        shader_char = (char*)malloc(total_size + 1);
        fread(shader_char, 1, total_size, shader_file);
        shader_char[total_size] = 0;
        fclose(shader_file);

        GLuint shader_id = 0;
        shader_id = glCreateShader(shader_type);
        glShaderSource(shader_id, 1, (const char**)&shader_char, NULL);
        glCompileShader(shader_id);

        GLint shader_compiled;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);
        if(shader_compiled != GL_TRUE)
        {
            GLsizei log_length;
            GLchar log[1024];
            glGetShaderInfoLog(shader_id, sizeof(log), &log_length, log);
            printf("Error in %s: %s\n", shader_file_name, log);
        }
        else
        {
            return shader_id;
        }
        }
        else
        {
            printf("Unable to open file %s\n", shader_file_name);
        }
        return -1;
}

// checks for shader program errors
void check_for_errors(int shader_program)
{
    int worked;
    char info_log[1024];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &worked);
    if(!worked)
    {
        glGetShaderInfoLog(shader_program, 1024, NULL, info_log);
        printf("Program linking failed: %s\n", info_log);
    }
}

// takes the already loaded shaders and attaches them to the shader program then deletes them (debugs if necessary) and finally returns the shader program
unsigned int load_glsl_shaders(const char* frag_shader_path, const char* vert_shader_path)
{
    unsigned int fragment, vertex, tess_control, tess_eval, program;
    fragment = load_shader_file(frag_shader_path, GL_FRAGMENT_SHADER);
    vertex = load_shader_file(vert_shader_path, GL_VERTEX_SHADER);
    //tess_control = load_shader_file(tess_control_path, GL_TESS_CONTROL_SHADER);
    //tess_eval = load_shader_file(tess_eval_path, GL_TESS_EVALUATION_SHADER);
    program = glCreateProgram();
    glAttachShader(program, fragment);
    glAttachShader(program, vertex);
    //glAttachShader(program, tess_control);
    //glAttachShader(program, tess_eval);
    glLinkProgram(program);
    check_for_errors(program);
    glDeleteShader(fragment);
    glDeleteShader(vertex);
    //glDeleteShader(tess_control);
    //glDeleteShader(tess_eval);
    return program;
}

// compiles the shaders and shader program (also debugs if necessary) then returns the shader program
unsigned int load_embedded_shaders(const char* vertex_shader_text, const char* fragment_shader_text)
{
    GLchar vertex_shader, fragment_shader, shader_program;
    int compiled;
    char info_log[512];

    // compile fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("Fragment shader failed: %s\n", info_log);
    }

    // compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("Vertex shader failed: %s\n", info_log);
    }

    /*
    // compile tessellation control shader
    tess_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tess_control_shader, 1, &tess_control_text, NULL);
    glCompileShader(tess_control_shader);
    glGetShaderiv(tess_control_shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        glGetShaderInfoLog(tess_control_shader, 512, NULL, info_log);
        printf("Tessellation control shader failed: %s\n", info_log);
    }
    */
    
    /*
    // compile tessellation evaluation shader
    tess_eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tess_eval_shader, 1, &tess_eval_text, NULL);
    glCompileShader(tess_eval_shader);
    glGetShaderiv(tess_eval_shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        glGetShaderInfoLog(tess_eval_shader, 512, NULL, info_log);
        printf("Tessellation evaluation shader failed: %s\n", info_log);
    }
    */

    // compile the shader program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, vertex_shader);
    //glAttachShader(shader_program, tess_control_shader);
    //glAttachShader(shader_program, tess_eval_shader);
    glLinkProgram(shader_program);
    check_for_errors(shader_program);

    // deletes the shaders
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    //glDeleteShader(tess_control_shader);
    //glDeleteShader(tess_eval_shader);

    // return shader program
    return shader_program;
}

// deletes the shader program
void shader_clean_up(unsigned int shader_program)
{
    glDeleteProgram(shader_program);
}

// uses the shader program
void use_shader(unsigned int shader_program)
{
    glUseProgram(shader_program);
}

// sets boolean values in the shader program / shader(s) that are linked to it
void set_bool(unsigned int shader_program, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), (int)value);
}

// sets integer values in the shader program / shader(s) that are linked to it
void set_int(unsigned int shader_program, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader_program, name), value);
}

// sets floating point values in the shader program / shader(s) that are linked to it
void set_float(unsigned int shader_program, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shader_program, name), value);
}

// sets 2 dimensional matrices in the shader program / shader(s) that are linked to it
void set_mat_2(unsigned int shader_program, const char* name, const mat2 mat)
{
    glUniformMatrix2fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, &mat[0][0]);
}

// sets 3 dimensional matrices in the shader program / shader(s) that are linked to it
void set_mat_3(unsigned int shader_program, const char* name, const mat3 mat)
{
    glUniformMatrix3fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, &mat[0][0]);
}

// sets 4 dimensional matrices in the shader program / shader(s) that are linked to it
void set_mat_4(unsigned int shader_program, const char* name, const mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, &mat[0][0]);
}

// sets 2 dimensional vectors in the shader program / shader(s) that are linked to it
void set_vec_2(unsigned int shader_program, const char* name, const vec2 value)
{
    glUniform2fv(glGetUniformLocation(shader_program, name), 1, &value[0]);
}

// sets 2 dimensional vectors with 3d space directions in the shader program / shader(s) that are linked to it
void set_vec_2_xyz(unsigned int shader_program, const char* name, float x, float y)
{
    glUniform2f(glGetUniformLocation(shader_program, name), x, y);
}

// sets 3 dimensional vectors in the shader program / shader(s) that are linked to it
void set_vec_3(unsigned int shader_program, const char* name, const vec3 value)
{
    glUniform3fv(glGetUniformLocation(shader_program, name), 1, &value[0]);
}

// sets 3 dimensional vectors with 3d space directions in the shader program / shader(s) that are linked to it
void set_vec_3_xyz(unsigned int shader_program, const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(shader_program, name), x, y, z);
}

// sets 4 dimensional vectors in the shader program / shader(s) that are linked to it
void set_vec_4(unsigned int shader_program, const char* name, const vec4 value)
{
    glUniform4fv(glGetUniformLocation(shader_program, name), 1, &value[0]);
}

// sets 4 dimensional vectors with 3d space directions that include the w (the component that scales the other dimensions) in the shader program / shader(s) that are linked to it
void set_vec_4_wyzx(unsigned int shader_program, const char* name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(shader_program, name), x, y, z, w);
}
#endif // SHADER_H_INCLUDED


