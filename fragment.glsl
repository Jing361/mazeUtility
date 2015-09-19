#version 330 core

struct Material{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;

//color factors
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
//attenuation factors
  float constant;
  float linear;
  float quadratic;
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
    Light lite = light[i];
    
    vec3 lightDir = normalize(lite.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
  
    vec3 ambient = lite.ambient * vec3(texture(material.diffuse, TexCoord));
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * (diff * lite.diffuse);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * lite.specular;
    
    float distance = length(lite.position - FragPos);
    //1 / constant + linear * distance + quadratic * distance^2
    float attenuation = 1.0f/(lite.constant + lite.linear * distance + lite.quadratic * (distance * distance));
    //float attenuation = 1.0f/(1.0 + 0.0014 * distance + 0.000007 * (distance * distance));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    result += (ambient + diffuse + specular);
  }
  
  color = vec4(result, 1.0f);
}
