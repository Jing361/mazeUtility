#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 lightPos;

void main(){
  //combine color and texture data
  //color = texture(ourTexture0, TexCoord) * vec4(ourColor, 1.0f);
  //combine 2 textures
	//color = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, TexCoord), 0.2);
  
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;
  
  vec3 result = (ambient + diffuse) * objColor;
  color = vec4(result, 1.0f);
}
