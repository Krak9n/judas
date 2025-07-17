#include "utils.h"
#include "deps.h"

#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
//#define NK_GLFW_VULKAN_IMPLEMENTATION
#include "nuklear/nuklear.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "shaders/opengl_stuff/shader2.h"

#define MAX_CHARACTERS 128 // ASCII range
#define MAX_SIZE 100

struct integers
{
  unsigned int VBO, VAO, EBO;
  unsigned int lightVAO;
  unsigned int VBO_plane, VAO_plane, EBO_plane;
} INTs;

struct loggs1
{
  char name[15];
  int success;
  char infoLog[512]; 
} loggs;

const unsigned int height = 900;
const unsigned int width = 1500;

/* CAMERA */
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.1f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.1f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

/* OBJECT PARAMETRS */
/* ----------- */
float vertices[] =
{
  //position        //colors         //texture coordinates
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

// vector for triangles
glm::vec3 posTriangles[] = 
{
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, 4.2f, -2.5f),
  glm::vec3(-3.8f, 2.0f, -12.3f),
  glm::vec3( 2.4f, 0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, 7.0f, -2.5f),
  glm::vec3( 1.5f,  2.6f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

float plane_coordinates[] = 
{
  // top one
  // coordinate     // color
  5.0f, 0.0f, 5.0f,  //far right 
  -5.0f, 0.0f, 5.0f, //far left
  -5.0f, 0.0f, -5.0f, //close left
  5.0f, 0.0f, -5.0f, //close right

};

unsigned int pl_ind[] = 
{
  0, 1, 2, 
  0, 2, 3,
};

/* ------------------- */

// lightning
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void init_glfw();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void freetype_loader();

static void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main(int argc, char* argv[]) 
{
  double xpos, ypos;
  // initializing GLFW
  init_glfw();

  struct nk_context *ctx;
  struct nk_colorf bg;
  struct nk_image img;
  
  // Create an application window with the following settings:
  static GLFWwindow* window = glfwCreateWindow(
      width, 
      height, 
      "judas", 
      NULL, 
      NULL);  
  if(window == NULL)
  {
    std::cout << "failed to create glfw\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // to be notified when cursor moves over the window
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSwapInterval(1);
  glfwSetCursorPos(window, xpos, ypos);

  // GLAD initializing
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "failed to initialize glad\n";
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  Shader objectsShader("src/shaders/opengl_stuff/vert_objects.glsl", "src/shaders/opengl_stuff/frag_objects.glsl");
  Shader light("src/shaders/opengl_stuff/vert_light.glsl", "src/shaders/opengl_stuff/frag_light.glsl");

  //binding all three vao, ebo and vbo
  glGenVertexArrays(1, &INTs.VAO);
  glGenBuffers(1, &INTs.EBO);
  glGenBuffers(1, &INTs.VBO);
  
  //drawing triangles
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
  /* ---------------- */

  //plane
  /* ---------------- */
  glGenVertexArrays(1, &INTs.VAO_plane);
  glGenBuffers(1, &INTs.EBO_plane);
  glGenBuffers(1, &INTs.VBO_plane);
  
  //drawing 
  glBindVertexArray(INTs.VAO_plane);
  glBindBuffer(GL_ARRAY_BUFFER, INTs.VBO_plane);
  glBufferData(GL_ARRAY_BUFFER, sizeof(plane_coordinates), plane_coordinates, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INTs.EBO_plane);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pl_ind), pl_ind, GL_STATIC_DRAW);

  //postions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  while (!glfwWindowShouldClose(window)) 
  {
    processInput(window);

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                           NK_WINDOW_BORDER |
                           NK_WINDOW_MOVABLE |
                           NK_WINDOW_SCALABLE |
                           NK_WINDOW_MINIMIZABLE |
                           NK_WINDOW_TITLE)) {
      // ..
      nk_layout_row_static(ctx, 30, 80, 1);
      if (nk_button_label(ctx, "button"))
      {
        fprintf(stdout, "button pressed\n");
      }
      // ..
    }
    light.use();
    light.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    light.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    light.setVec3("lightPos", lightPos);

    
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)(width/height), 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    
    light.setMat4("projection", projection);
    light.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    light.setMat4("model", model);    

    /* THE PLANE */
    glBindVertexArray(INTs.VAO_plane);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    objectsShader.use();
    objectsShader.setMat4("projection", projection); 
    objectsShader.setMat4("view", view);
    
    // render triangles
    glBindVertexArray(INTs.VAO); 
    for (unsigned int i = 0; i < 10; i++)
    {
      // calculate the model matrix for each object and pass it to shader before drawing
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, posTriangles[i]);
      model = glm::translate(model, lightPos);
      float angle = 20.0f + i;
      model = glm::rotate(model, (float)(glfwGetTime() + i) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      objectsShader.setMat4("model", model);

      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    // Do game logic, present a frame, etc.
  }
  
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwDestroyWindow(window);
 
  // Close and destroy the window
  glDeleteVertexArrays(1, &INTs.VAO);
  glDeleteBuffers(1, &INTs.VBO);
  
  // Clean up
  glfwTerminate();
  
  return 0;
}

// return here 
static void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
{
  glfwGetCursorPos(window, &xposIn, &yposIn);
  std::cout << "\nmouse position is,\nx: " << xposIn << "\ny: " << yposIn;
  
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse)
  {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
      pitch = 89.0f;
  if (pitch < -89.0f)
      pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);

}

void processInput(GLFWwindow *window)
{
  float cameraSpeed = static_cast<float>(2.5 * deltaTime);
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  // moving around
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    cameraPos += cameraSpeed * cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    cameraPos -= cameraSpeed * cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    cameraPos += cameraUp / 50.0f;
  }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth)
{
  glViewport(0, 0, width, heigth);
}

// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  fov -= (float)yoffset;
  if (fov < 1.0f)
  {
    fov = 1.0f;
  }
  if (fov > 45.0f)
  {
    fov = 45.0f;
  }
}

void init_glfw()
{
  glfwInit();  

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

}
