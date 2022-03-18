#pragma once

class FragmentShader
{
public:
    static const char* fragmentShader;
};

const char* FragmentShader::fragmentShader = R"(
    #version 440 core
   
   out vec4 frag_Output;

   void main(void)
   {
      frag_Output = vec4(1.0f, 0.0f, 0.0f, 1.0f);
   }
)";