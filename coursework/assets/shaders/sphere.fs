#version 430 core


in vec4 tesWorldPos;
in vec3 tesColor;

out vec4 FragColor;


uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform int ambient;
uniform int diffuse;


void main()
{
	// ambient
	vec3 ambientVec = vec3(0.0f);
	if(ambient == 1)
	{
		float ambientStrength = 0.15f;
    	ambientVec = ambientStrength * lightColor;			
	}

    // diffuse 
    vec3 diffuseVec = vec3(0.0f);
    if(diffuse == 1)
    {
    	vec3 normal   = normalize(tesWorldPos.xyz);
    	vec3 lightDir = normalize(lightPos - tesWorldPos.xyz);

    	float diff = max(dot(normal, lightDir), 0.0f);
    	diffuseVec = diff * lightColor;	
    }

    //result    
    vec3 result = (ambientVec + diffuseVec) * tesColor;
    FragColor = vec4(result, 1.0);
}