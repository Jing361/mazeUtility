#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform vec3 lightColor;
uniform vec3 objColor;

void main(){
  //combine color and texture data
  //color = texture(ourTexture0, TexCoord) * vec4(ourColor, 1.0f);
  //combine 2 textures
	//color = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, TexCoord), 0.2);
  
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;
  
  vec3 result = ambient * objColor;
  color = vec4(result, 1.0f);
}
