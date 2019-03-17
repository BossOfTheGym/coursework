#version 430 core

in vec4 tesModelPos;
in vec4 tesWorldPos;
in vec3 tesNormal;
in vec3 tesColor;

out vec4 FragColor;


uniform vec3 lightPos; 
uniform vec3 lightColor;

uniform vec3 color;


void main()
{
	// ambient
	vec3 ambientVec;
	float ambientStrength = 0.08f;
	ambientVec = ambientStrength * lightColor;			

    // diffuse 
    vec3 diffuseVec;
    vec3 lightDir = normalize(lightPos - tesWorldPos.xyz);

    float diff = max(dot(tesNormal, lightDir), 0.0f);
    diffuseVec = diff * lightColor;	

    //resulting light
    vec3 light = ambientVec + diffuseVec;


	//result
    FragColor = vec4(light * color, 1.0f);
}