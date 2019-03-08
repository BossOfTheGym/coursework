#version 430 core

in vec4 tesModelPos;
in vec4 tesWorldPos;
in vec3 tesColor;
in vec3 tesNormal;

out vec4 FragColor;


uniform vec3 lightPos; 
uniform vec3 lightColor;

uniform sampler2D mapMain;
uniform sampler2D specularMap;
uniform sampler2D mapBump;

const float PI = acos(-1.0f); 
const float PI_2 = 2 * PI;

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
	texCoord.t = clamp(1.0f - latitude / PI   , 0.0f, 1.0f);

	//result
    vec4 tex = texture(mapMain, texCoord);
    FragColor = vec4(clamp(light, vec3(0.0f), vec3(1.0f)) * tex.xyz, 1.0f);
}