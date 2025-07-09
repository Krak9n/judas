#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "shader2.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// firstly declaration and only then importing
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

#define MAX_CHARACTERS 128 // ASCII range
#define MAX_SIZE 100

SDL_Event event;

struct integers
{
  unsigned int VBO, VAO, EBO;
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

  0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, // far right bottom
  -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, // far left bottom
  0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom face to me
  0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f, // top point
};

unsigned int indices[] =
{
  0, 1, 2, //plane
  1, 0, 3, // behind
  0, 2, 3, // the right one
  1, 2, 3// the left one

};

glm::vec3 posTriangles[] = 
{
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
/* ------------------- */

int main(int argc, char* argv[]) 
{

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
  Shader ourShader("src/shaders/vert.glsl", "src/shaders/frag.glsl");
  
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
  ourShader.use();
  
  while (!done) 
  {
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_EVENT_QUIT) 
        {
          done = true;
        }
    }
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ourShader.use();
    
    // rotating stuff 
    glm::mat4 view =  glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);   

    projection = glm::perspective(glm::radians(45.0f), (float)(width/height), 0.1f, 100.0f);
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // pass transformation matrices to the shader
    ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    ourShader.setMat4("view", view);

    // render 
    glBindVertexArray(INTs.VAO); 
    for (unsigned int i = 0; i < 10; i++)
    {
      // calculate the model matrix for each object and pass it to shader before drawing
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, posTriangles[i]);
      float angle = 20.0f + i;
      model = glm::rotate(model, (SDL_GetTicks() / 100.0f) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      ourShader.setMat4("model", model);

      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    }
   
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

