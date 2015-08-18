#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

//out vec3 objColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main(){
  gl_Position = projection * view * transform * vec4(position, 1.0);
  //objColor = color;
  TexCoord = texCoord;
}
