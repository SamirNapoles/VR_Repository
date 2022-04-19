#pragma once

class VertexShader
{
public:
    static const char* vertexShader;
    static const char* passthroughVertexShader;
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

const char* VertexShader::passthroughVertexShader = R"(
   #version 440 core

   // Uniforms:
   uniform mat4 projection;
   uniform mat4 modelview;   

   // Attributes:
   layout(location = 0) in vec3 in_Position;   
   layout(location = 2) in vec2 in_TexCoord;

   // Varying:   
   out vec2 texCoord;

   void main(void)
   {      
      gl_Position = projection * modelview * vec4(in_Position, 1.0f);    
      texCoord = in_TexCoord;
   }
)";