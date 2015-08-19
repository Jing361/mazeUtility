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
uniform vec3 viewPos;

void main(){
  //combine color and texture data
  //color = texture(ourTexture0, TexCoord) * vec4(ourColor, 1.0f);
  //combine 2 textures
	//color = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, TexCoord), 0.2);
  
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;
  
  float diffuseStrengeth = 1.0f;
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diffuseStrengeth * diff * lightColor;
  
  float specularStrength = 0.5f;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;
  //specular = vec3(0.0, 0.0, 0.0);
  
  vec3 result = (ambient + diffuse + specular) * objColor;
  color = vec4(result, 1.0f);
}
