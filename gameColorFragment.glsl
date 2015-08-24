#version 330 core

struct Material{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 Color;

out vec4 color;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){
  color = vec4(Color, 1.0f);
}
