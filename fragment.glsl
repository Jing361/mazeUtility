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

out vec4 color;

uniform Material material;
uniform Light light[5];
uniform vec3 viewPos;

void main(){
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  
  vec3 result = vec3(0.0, 0.0, 0.0);
  
  for(int i = 0; i < 5; ++i){
    vec3 lightDir = normalize(light[i].position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
  
    vec3 ambient = light[i].ambient * vec3(texture(material.diffuse, TexCoord));
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * (diff * light[i].diffuse);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * light[i].specular;
    
    result = result + ambient + diffuse + specular;
  }
  color = vec4(result, 1.0f);
}
