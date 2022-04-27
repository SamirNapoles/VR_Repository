#pragma once

class FragmentShader
{
public:
    static const char* fragmentShaderOmni;
    static const char* fragmentShaderDirectional;
    static const char* fragmentShaderSpot;
    static const char* passthroughFragmentShader;
    static const char* skyBoxFragmentShader;
};

/*
const char* FragmentShader::fragmentShaderOmni = R"(
    #version 440 core

   in vec4 fragmentPosition;
   in vec3 normal;
   in vec2 texCoord;
   
   out vec4 frag_Output;

   // Material properties
   uniform vec3 materialEmission;
   uniform vec3 materialAmbient;
   uniform vec3 materialDiffuse;
   uniform vec3 materialSpecular;
   uniform float materialShiniess;

   // Light properties
   uniform vec3 lightPosition;
   uniform vec3 lightAmbient;
   uniform vec3 lightDiffuse;
   uniform vec3 lightSpecular;

   // Texture mapping:
   layout(binding = 0) uniform sampler2D texSampler;

   void main(void)
   {
      // Texture element:
      vec4 texel = texture(texSampler, texCoord);

      // Ambient
      vec3 fragmentColor = materialEmission + materialAmbient * lightAmbient;

      vec3 normalizedNormal = normalize(normal);
      vec3 lightDirection = normalize(lightPosition - fragmentPosition.xyz);
      float normalDotLightDirection = dot(normalizedNormal, lightDirection);

      // only if lighted
      if (normalDotLightDirection > 0.0f) {
         // Diffuse
         fragmentColor += materialDiffuse * normalDotLightDirection * lightDiffuse;

         vec3 halfVector = normalize(lightDirection + normalize(-fragmentPosition.xyz));
         float halfDotNormal = dot(halfVector, normalizedNormal);

         // Specular
         fragmentColor += materialSpecular * pow(halfDotNormal, materialShiniess) * lightSpecular;
      }

      frag_Output = texel * vec4(fragmentColor, 1.0f);
      // frag_Output = vec4(1.0f, 1.0f, 1.0f, 1.0f);
   }
)";

const char* FragmentShader::fragmentShaderDirectional = R"(
    #version 440 core

   in vec4 fragmentPosition;
   in vec3 normal;
   in vec2 texCoord;
 
   out vec4 frag_Output;

   // Material properties
   uniform vec3 materialEmission;
   uniform vec3 materialAmbient;
   uniform vec3 materialDiffuse;
   uniform vec3 materialSpecular;
   uniform float materialShiniess;

   // Light properties
   uniform vec3 lightDirection;
   uniform vec3 lightAmbient;
   uniform vec3 lightDiffuse;
   uniform vec3 lightSpecular;

   // Texture mapping:
   layout(binding = 0) uniform sampler2D texSampler;

   void main(void)
   {
      // Texture element:
      vec4 texel = texture(texSampler, texCoord);

      // Ambient
      vec3 fragmentColor = materialEmission + materialAmbient * lightAmbient;

      vec3 normalizedNormal = normalize(normal);
      vec3 normalizedDirection = normalize(lightDirection);
      float normalDotLightDirection = dot(normalizedNormal, normalizedDirection);

      // only if lighted
      if (normalDotLightDirection > 0.0f) {
         // Diffuse
         fragmentColor += materialDiffuse * normalDotLightDirection * lightDiffuse;

         vec3 halfVector = normalize(normalizedDirection + normalize(-fragmentPosition.xyz));
         float halfDotNormal = dot(halfVector, normalizedNormal);

         // Specular
         fragmentColor += materialSpecular * pow(halfDotNormal, materialShiniess) * lightSpecular;
      }

      frag_Output = texel * vec4(fragmentColor, 1.0f);
      // frag_Output = vec4(1.0f, 1.0f, 1.0f, 1.0f);
   }
)";

const char* FragmentShader::fragmentShaderSpot = R"(
    #version 440 core

   in vec4 fragmentPosition;
   in vec3 normal;
   in vec2 texCoord;

   out vec4 frag_Output;

   // Material properties
   uniform vec3 materialEmission;
   uniform vec3 materialAmbient;
   uniform vec3 materialDiffuse;
   uniform vec3 materialSpecular;
   uniform float materialShiniess;

   // Light properties
   uniform vec3 lightPosition;
   uniform vec3 lightDirection;
   uniform vec3 lightAmbient;
   uniform vec3 lightDiffuse;
   uniform vec3 lightSpecular;
   uniform float cutOff;

   // Texture mapping:
   layout(binding = 0) uniform sampler2D texSampler;    

   void main(void)
   {
      // Texture element:
      vec4 texel = texture(texSampler, texCoord);

      // Ambient
      vec3 fragmentColor = materialEmission + materialAmbient * lightAmbient;

      vec3 normalizedNormal = normalize(normal);
      vec3 fragmentPositionToLight = normalize(lightPosition - fragmentPosition.xyz);
      float normalDotFragmentToLight = dot(normalizedNormal, fragmentPositionToLight);

      float fragmentToLightDotDirection = dot(fragmentPositionToLight, -lightDirection);

      // only if lighted
      if ((normalDotFragmentToLight > 0.0f) && (fragmentToLightDotDirection > cos(cutOff))) { //
         // Diffuse
         fragmentColor += materialDiffuse * normalDotFragmentToLight * lightDiffuse;

         vec3 halfVector = normalize(fragmentPositionToLight + normalize(-fragmentPosition.xyz));
         float halfDotNormal = dot(halfVector, normalizedNormal);

         // Specular
         fragmentColor += materialSpecular * pow(halfDotNormal, materialShiniess) * lightSpecular;
      }

      frag_Output = texel * vec4(fragmentColor, 1.0f);
      // frag_Output = vec4(1.0f, 1.0f, 1.0f, 1.0f);
      fragmentColor += vec3(cutOff);
   }
)";

const char* FragmentShader::skyBoxFragmentShader = R"(
   #version 440 core
   
   in vec3 texCoord;
   
   // Texture mapping (cubemap):
   layout(binding = 0) uniform samplerCube cubemapSampler;

   out vec4 fragOutput;

   void main(void)
   {       
      fragOutput = texture(cubemapSampler, texCoord);
   }
)";
*/
)";

const char* FragmentShader::passthroughFragmentShader = R"(
   #version 440 core
   
   in vec2 texCoord;
   
   uniform vec4 color;

   out vec4 fragOutput;   

   // Texture mapping:
   layout(binding = 0) uniform sampler2D texSampler;

   void main(void)   
   {  
      // Texture element:
      vec4 texel = texture(texSampler, texCoord);      
      
      // Final color:
      fragOutput = color * texel;       
   }
)";