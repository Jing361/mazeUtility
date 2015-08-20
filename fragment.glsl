#version 330 core

struct Material{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){
  //combine color and texture data
  //color = texture(ourTexture0, TexCoord) * vec4(ourColor, 1.0f);
  //combine 2 textures
	//color = mix(texture(ourTexture0, TexCoord), texture(ourTexture1, TexCoord), 0.2);
  
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  
  vec3 ambient = light.ambient * material.ambient;
  
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = material.diffuse * (diff * light.diffuse);
  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = (material.specular) * spec * light.specular;
  
  vec3 result = ambient + diffuse + specular;
  color = vec4(result, 1.0f);
}
