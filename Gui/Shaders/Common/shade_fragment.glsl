//--------------------------------------------------------------------------------------
// Order Independent Transparency Fragment Shader
//
// Author: Théo Devaucoup
//--------------------------------------------------------------------------------------

#version 330 core

uniform float Alpha;
uniform vec3 Color;

in vec3 normal;
in vec3 lightDir;

uniform bool UseAmbiantLight;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform Material material;
uniform bool materialOn;

vec3 Ambient;
vec3 Diffuse;
vec3 Specular;

// function prototypes
void CalcDirLight(DirLight p_light, vec3 p_normal, vec3 p_viewDir);
void CalcSpotLight(SpotLight p_light, vec3 p_normal, vec3 p_lightDir, vec3 p_viewDir);

vec4 ShadeFragment()
{
    vec4 color;
 
    if (UseAmbiantLight)
    {
        vec3 viewDir = normalize(-lightDir);
        
        // Clear the light intensity accumulators
        Ambient  = vec3 (0.0);
        Diffuse  = vec3 (0.0);
        Specular = vec3 (0.0);
        
        // == =====================================================
        // Our lighting is set up in 2 phases: directional and a spot light
        // For each phase, a calculate function is defined that calculates the corresponding color
        // per lamp. In the main() function we take all the calculated colors and sum them up for
        // this fragment's final color.
        // == =====================================================
        // phase 1: directional lighting
        CalcDirLight(dirLight, normal, viewDir);
        
        // phase 2: spot light
        CalcSpotLight(spotLight, normal, lightDir, viewDir);

        vec3 tmpColor;
        if (materialOn) // useful for rendering scapula
        {
            vec3 sceneColor = material.ambient * vec3(0.4f, 0.2f, 0.2f);
            
            tmpColor = sceneColor +
                   Ambient  * material.ambient +
                   Diffuse  * material.diffuse +
                   Specular * material.specular;
        }
        else
        {
            tmpColor = Ambient  * Color +
                    Diffuse  * Color +
                    Specular * Color;
        }
        color.rgb = clamp(tmpColor, 0.0, 1.0);
    }
    else
    {
        float diffuse = abs(dot(normal, lightDir));
        color.rgb = Color * (0.42 + 0.58 * diffuse);
    }

    color.a = Alpha;
    return color;
}

// calculate the color when using a directional light
void CalcDirLight(DirLight p_light, vec3 p_normal, vec3 p_viewDir)
{
    vec3 direction = vec3(0.f, 0.f, -1.f);
    vec3 vLightDir = normalize(-direction);
    // diffuse shading
    float diff = max(dot(p_normal, vLightDir), 0.0f);
    // specular shading
    vec3 reflectDir = reflect(-vLightDir, p_normal);
    // Mantis bug M#0000348, the following lines causes bad rendering on Intel HD 4000 family.
    // Reactivate with 'matShininess' value different than 0.0f for a different specular effect.
    // Do not calculate pow(x, 0.0) as it causes problem with older graphical cards.
    // float matShininess = 0.0f;
    // float spec = pow(max(dot(p_viewDir, reflectDir), 0.0f), matShininess);    
    float spec = 1.;
    // combine results
    Ambient  += p_light.ambient;
    Diffuse  += p_light.diffuse * diff;
    Specular += p_light.specular * spec;
}

// calculate the color when using a spot light
void CalcSpotLight(SpotLight p_light, vec3 p_normal, vec3 p_lightDir, vec3 p_viewDir)
{
    vec3 vLightDir = normalize(p_light.position - p_lightDir);
    // diffuse shading
    float diff = max(dot(p_normal, vLightDir), 0.0f);
    // specular shading
    vec3 reflectDir = reflect(-vLightDir, p_normal);
    // Mantis bug M#0000348, same as above
    // float matShininess = 0.0f;
    // float spec = pow(max(dot(p_viewDir, reflectDir), 0.0f), matShininess);
    float spec = 1.;    
    // attenuation
    float distance = length(p_light.position - p_lightDir);
    float attenuation = 1.0f / (p_light.constant + p_light.linear * distance + p_light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(vLightDir, normalize(-p_light.direction)); 
    float epsilon = p_light.cutOff - p_light.outerCutOff;
    float intensity = clamp((theta - p_light.outerCutOff) / epsilon, 0.0f, 1.0f);
   
    // combine results
    Ambient  += p_light.ambient * attenuation * intensity;
    Diffuse  += p_light.diffuse * diff * attenuation * intensity;
    Specular += p_light.specular * spec * attenuation * intensity;
}
