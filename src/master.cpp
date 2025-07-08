#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "shader_m.h"
//#include "get_shaders.h"

#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include "cglm/cglm.h"

// firstly declaration and only then importing
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define MAX_CHARACTERS 128 // ASCII range
#define MAX_SIZE 100

SDL_Event event;

struct Global
{
  int fuck;
};

struct integers
{
  unsigned int VBO, VAO, EBO;
  unsigned int texture1, texture2;
} INTs;

struct loggs1
{
  char name[15];
  int success;
  char infoLog[512]; 
} loggs;

const unsigned int height = 900;
const unsigned int width = 1500;

/* OBJECT PARAMETRS */
/* ----------- */
float vertices[] =
{
  //position        //colors         //texture coordinates
  /* PLANE */

  0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 
  -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 
  0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 
  0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
};

unsigned int indices[] =
{
  0, 1, 2, //plane
  0, 1, 3, // behind
  0, 2, 3, // the right one
  1, 2, 3// the left one

};
/* ------------------- */

/* SHADERS */
/* ------------------ */
const char *VshaderSource =
  "#version 440\n"
  "layout (location = 0) in vec4 aPos;\n"
  "layout (location = 1) in vec4 inColor;\n"

  "out vec4 exColor;\n"

  "uniform mat4 model;\n"
  "uniform mat4 projection;\n"
  "uniform mat4 view;\n"
  
  "void main()\n"
  "{\n"
  "   gl_Position = (projection * view * model) * aPos;\n"
  "   exColor = inColor;\n"
  "}\0";


const char *FshaderSource =
  "#version 440\n"
  "in vec4 exColor;\n"
  
  "out vec4 outColor;\n"
  
  "void main()\n"
  "{\n"
    "outColor = exColor;\n"
  "}\0";
/* --------------------- */

//bool SDL_GL_MakeCurrent(SDL_Window *window, SDL_GLContext context);
//void shader_get();
//void linking_shaders();
//char* read_shader_source(const char* filePath);
//GLuint compile_shader(GLenum type, const char* source);
//void init_shaders();

int main(int argc, char* argv[]) 
{

  // struct Global declaring
  struct Global gl_var;
  
  SDL_Window *window;                  
  bool done = false;

  // initializing SDL3
  SDL_Init(SDL_INIT_VIDEO);            
  // Create an application window with the following settings:
  window = SDL_CreateWindow(
      "judas",                  
      width,                       
      height,               
      SDL_WINDOW_OPENGL                
  );
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // Check that the window was successfully created
  if (window == NULL) 
  {
      // In the case that the window could not be made...
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
      return 1;
  }
  
  // an OpenGL context
  SDL_GLContext glcontext = SDL_GL_CreateContext(window); 
  // making window current
  bool SDL_GL_MakeCurrent(SDL_Window *window, SDL_GLContext context);

  // GLAD initializing
  if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    printf("failed to initialize glad\n");
    return -1;
  }
  
  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  unsigned int shader_program = load_glsl_shaders("src/shaders/frag.glsl", "src/shaders/vert.glsl");

  // generating vertex and fragment shaders
  //shader_get();
  //linking ones before
  //linking_shaders();

  //binding all three vao, ebo and vbo
  glGenVertexArrays(1, &INTs.VAO);
  glGenBuffers(1, &INTs.EBO);
  glGenBuffers(1, &INTs.VBO);
  
  //drawing two triangles
  glBindVertexArray(INTs.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, INTs.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INTs.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  
  //postions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  //color of vertices
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  
  //init_shaders();
  use_shader(shader_program);

  while (!done) 
  {

    uint32_t startTime = SDL_GetTicks();

    //after currTime
    //double elapsedTime = (currTime - startTime) / 1000.0; // Convert to seconds.

    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_EVENT_QUIT) 
        {
          done = true;
        }
    }
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    use_shader(shader_program);

    // rotating stuff 
    mat4 model =  {
      {1.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f}
    };
    mat4 view =  {
      {1.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f}
    };
    mat4 projection =  {
      {1.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f}
    };
    
    uint32_t currTime = SDL_GetTicks();
    float elapsedTime = (currTime - startTime) / 1000.0; 

    vec3 ve2 = { 0.5f, 1.0f, 0.0f };
    glm_vec3_normalize(ve2);
    glm_rotate_make(model, elapsedTime, ve2);
    
    vec3 vv = { 0.0f, 0.0f, -3.0f };
    glm_vec3_normalize(vv);
    glm_translate_make(view, vv);
    
    glm_perspective_resize((float)(width/height), projection);
    
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
    unsigned int viewLoc  = glGetUniformLocation(shader_program, "view");
    
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)&model);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)&view);
    set_mat_4(shader_program, "projection", projection);

    glBindVertexArray(INTs.VAO); 
    glDrawArrays(GL_TRIANGLES, 0, 12);

    SDL_GL_SwapWindow(window);
    // Do game logic, present a frame, etc.
  }

  // Close and destroy the window
  SDL_GL_DestroyContext(glcontext);  
  SDL_DestroyWindow(window);
  glDeleteVertexArrays(1, &INTs.VAO);
  glDeleteBuffers(1, &INTs.VBO);
  
  // Clean up
  SDL_Quit();
  return 0;
}
/*
void shader_get()
{
  // vertex shader
  INTs.Vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(INTs.Vshader, 1, &VshaderSource, NULL);
  glCompileShader(INTs.Vshader);
    
  glGetShaderiv(INTs.Vshader, GL_COMPILE_STATUS, &loggs.success);
  // checks for errors while compiling
  if(!loggs.success)
  {
    glGetShaderInfoLog(INTs.Vshader, 512, NULL, loggs.infoLog);
    printf("error while compiling vertex shader %s\n", loggs.infoLog);
  }

  // fragment shader
  INTs.Fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(INTs.Fshader, 1, &FshaderSource, NULL);
  glCompileShader(INTs.Fshader);
 
  glGetShaderiv(INTs.Fshader, GL_COMPILE_STATUS, &loggs.success);
  if(!loggs.success)
  {
    glGetShaderInfoLog(INTs.Fshader, 512, NULL, loggs.infoLog);
    printf("error while compiling fragment shader %s\n", loggs.infoLog);
  }

}

void linking_shaders()
{
  // for the first triangle
  shader_get();

  // linking shaders 
  INTs.shaderProgram = glCreateProgram();
  glAttachShader(INTs.shaderProgram, INTs.Vshader);
  glAttachShader(INTs.shaderProgram, INTs.Fshader);
  glLinkProgram(INTs.shaderProgram);
  // checks for the errors while linking shaders   
  glGetProgramiv(INTs.shaderProgram, GL_LINK_STATUS, &loggs.success);
  if (!loggs.success) {
      glGetProgramInfoLog(INTs.shaderProgram, 512, NULL, loggs.infoLog);
      printf("error while linking %s\n", loggs.infoLog);
  }

  // also could add some checking for errors while compiling but anyway xDDD
  // deleting shaders 'cause those were already linked
  glDeleteShader(INTs.Vshader);
  glDeleteShader(INTs.Fshader);

}
*/
