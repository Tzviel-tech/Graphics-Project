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

	//ambient
	float ambientStrength = 0.3;
    vec3 ambient = vec3(0.19225,	0.19225,	0.19225) * lightcolor;
    
	//diffuse
	vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightP - fragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(0.50754	,0.50754,	0.50754)*diff * lightcolor;
	

	//specular
	vec3 camerapos =vec3(0, 0, 0);
	vec3 viewDir = normalize(camerapos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular =  vec3(0.508273	,0.508273	,0.508273) * spec * lightcolor;  

	vec3 result = (ambient + diffuse + specular) ;
    
    frag_color = vec4(norm, 1.0);
}
