#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
//#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "cglm/cglm.h"

// firstly declaration and only then importing
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define MAX_CHARACTERS 128 // ASCII range
#define MAX_SIZE 100

struct Global
{
  int ficl;
};

struct integers
{
  unsigned int VBO, VAO, EBO;
  unsigned int shaderProgram, Vshader, Fshader;
  unsigned int texture1, texture2;
  unsigned int shader_program;
} INTs;

struct loggs1
{
  int success;
  char infoLog[512]; 
} loggs;

struct vectors
{
  vec3 to_tr;
} v;

const unsigned int height = 900;
const unsigned int width = 1500;

/* OBJECT PARAMETRS */
/* ----------- */
float vertices[] =
{
  //position        //colors         //texture coordinates
  0.0f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 
  -0.4f, 0.0f, 0.0f, 0.0f, 0.4f, 0.0f,
  0.0f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f,
  0.4f, 0.0f, 0.0f, 0.2f, 0.08f, 0.0f,
};

unsigned int indices[] =
{
  0, 1, 2,
  0, 3, 2
};
/* ------------------- */

/* SHADERS */
/* ------------------ */
const char *VshaderSource =
  "#version 440 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec2 aTexCoord;\n"

  "out vec2 TexCoord;\n"

  "uniform mat4 model;\n"
  "uniform mat4 view;\n"
  "uniform mat4 projection;\n"

  "void main()\n"
  "{\n"
	  "gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	  "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
  "}\0";

const char *FshaderSource =
  "#version 330 core\n"
  "out vec4 FragColor;\n"

  "in vec2 TexCoord;\n"

  // texture samplers
  "uniform sampler2D texture1;\n"
  "uniform sampler2D texture2;\n"

  "void main()\n"
  "{\n"
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    "FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n"
  "}\0";
/*
const char *VshaderSource = 
    "#version 440 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

//used for drawing animation in which ourColor is a variable
const char *FshaderSource = 
    "#version 440 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\n\0";
*/
/* --------------------- */

//bool SDL_GL_MakeCurrent(SDL_Window *window, SDL_GLContext context);
void shader_get();
void linking_shaders();
char* read_shader_source(const char* filePath);
GLuint compile_shader(GLenum type, const char* source);
void init_shaders();

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
  
  // generating vertex and fragment shaders
  shader_get();
  //linking ones before
  linking_shaders();

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  //color of vertices
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  
  //init_shaders();

  while (!done) 
  {

    uint32_t startTime = SDL_GetTicks();

    //after currTime
    //double elapsedTime = (currTime - startTime) / 1000.0; // Convert to seconds.
    SDL_Event event;

    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_EVENT_QUIT) 
        {
          done = true;
        }
    }

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    /*
    // create transformations
    mat4 transform = { 1.0f };
    vec3 for_now = { 0.5f, -0.5f, 0.0f };
    glm_translate_make(transform, for_now);
    
    vec3 for_here = { 0.0f, 0.0f, 1.0f };
    uint32_t currTime = SDL_GetTicks();
    glm_rotate_make(transform, (float)currTime, for_here);
    //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    // get matrix's uniform location and set matrix
    unsigned int transformLoc = glGetUniformLocation(INTs.shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (float*)&transform);
    */
    glUseProgram(INTs.shaderProgram);

    mat4 model = { 1.0f };
    mat4 view = { 1.0f };
    mat4 projection = { 1.0f };

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    glBindVertexArray(INTs.VAO); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
    // Do game logic, present a frame, etc.
  }

  // Close and destroy the window
  SDL_GL_DestroyContext(glcontext);  
  SDL_DestroyWindow(window);

  // Clean up
  SDL_Quit();
  return 0;
}

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

/* TREE NEXT FUNCTIONS FOR LOADING FILE SHADERS */
/* -------------- */
// Function to read shader source from file
char* read_shader_source(const char* filePath) 
{
    FILE* file = fopen(filePath, "r");
    if (file == NULL) 
    {
        printf("failed to open file: %s\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(fileSize + 1);
    if (content == NULL) 
    {
        printf("failed to allocate memory for file content\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(content, 1, fileSize, file);
    if (bytesRead < fileSize) 
    {
        printf("failed to read file content\n");
        free(content);
        fclose(file);
        return NULL;
    }

    content[fileSize] = '\0';
    fclose(file);
    return content;
}

// Function to compile shader
GLuint compile_shader(GLenum type, const char* source) 
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    loggs.success;
    loggs.infoLog;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &loggs.success);
    if (!loggs.success) 
    {
        glGetShaderInfoLog(shader, 512, NULL, loggs.infoLog);
        printf("shader compilation failed\n%s\n", loggs.infoLog);
        return 0;
    }
    return shader;
}

void init_shaders() 
{
    // Read vertex shader from file
    char* vertex_shader_source = read_shader_source("src/shaders/transform.vs");
    if (vertex_shader_source == NULL) 
    {
        exit(1);
    }

    // Compile vertex shader
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    free(vertex_shader_source);
    if (!vertex_shader) exit(1);

    // Read and compile fragment shader
    char* fragment_shader_source = read_shader_source("src/shaders/transform.fs");
    if (fragment_shader_source == NULL) 
    {
        exit(1);
    }
    
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    free(fragment_shader_source);
    if (!fragment_shader) exit(1);

    // Link shaders
    INTs.shader_program = glCreateProgram();
    glAttachShader(INTs.shader_program, vertex_shader);
    glAttachShader(INTs.shader_program, fragment_shader);
    glLinkProgram(INTs.shader_program);

    // Check for linking errors
    glGetProgramiv(INTs.shader_program, GL_LINK_STATUS, &loggs.success);
    if (!loggs.success) 
    {
        glGetProgramInfoLog(INTs.shader_program, 512, NULL, loggs.infoLog);
        printf("shader linking failed\n%s\n", loggs.infoLog);
        exit(1);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}
/* -------------- */
