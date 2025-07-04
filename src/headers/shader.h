#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct shader
{
  unsigned int ID;
  // sign values later
  char vertexCode[];
  char fragmentCode[];

} Shader;

void love()
{
  Shader(const char* vertexPath, const char* fragmentPath)
  {
    Shader.vertexCode;
    Shader.fragmentCode;

    
  }
}
