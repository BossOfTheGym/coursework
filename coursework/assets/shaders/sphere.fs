#version 430 core

in vec4 tesModelPos;
in vec4 tesWorldPos;
in vec3 tesColor;
in vec3 tesNormal;

out vec4 FragColor;


uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform int ambient;
uniform int diffuse;

uniform sampler2D texture0;

const float PI = acos(-1.0f); 
const float PI_2 = 2 * PI;

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
    	vec3 lightDir = normalize(lightPos - tesWorldPos.xyz);

    	float diff = max(dot(tesNormal, lightDir), 0.0f);
    	diffuseVec = diff * lightColor;	
    }

    //resulting light
    vec3 light = ambientVec + diffuseVec;

    //texel obtainment
    vec3 spherePos = normalize(tesModelPos.xyz);
    vec2 circlePos = normalize(spherePos.xy);

    float latitude  = acos(spherePos.z);
    float longitude = acos(circlePos.x);
    if (circlePos.y < 0.0f)
    {
    	longitude = 2 * PI - longitude;
    }

    vec2 texCoord;
    texCoord.s = clamp(longitude / PI_2, 0.0f, 1.0f);
	texCoord.t = clamp(latitude / PI   , 0.0f, 1.0f);

    vec4 tex = texture(texture0, texCoord);
    FragColor = vec4(light * mix(tex.xyz, tesColor, 0.7f), 1.0f);
}