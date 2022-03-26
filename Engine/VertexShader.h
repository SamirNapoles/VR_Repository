#pragma once

class VertexShader
{
public:
    static const char* vertexShader;
};

const char* VertexShader::vertexShader = R"(
   #version 440 core

   uniform mat4 projection;
   uniform mat4 modelview;
   uniform mat3 modelviewInverseTranspose;

   layout(location = 0) in vec3 in_Position;
   layout(location = 1) in vec3 in_Normal;

   out vec4 fragmentPosition;
   out vec3 normal;

   void main(void)
   {
      fragmentPosition = modelview * vec4(in_Position, 1.0f);
      gl_Position = projection * fragmentPosition;
      normal = modelviewInverseTranspose * in_Normal;
   }
)";