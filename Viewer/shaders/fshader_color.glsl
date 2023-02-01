#version 330 core

struct Material
{
	sampler2D textureMap;
   
};

// We set this field's properties from the C++ code
uniform Material material;
uniform vec3 color;
uniform vec3 lightcolor;
uniform vec3 ambientcolor;



// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
in vec3 lightP;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	float ambientStrength = 0.7;
    vec3 ambient = ambientStrength * lightcolor;
    
	vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightP - fragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightcolor;
	vec3 result = (ambient + diffuse) * color;
    frag_color = vec4(result, 1.0);
}
