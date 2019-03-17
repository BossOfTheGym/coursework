#version 430 core

in vec4 geoModelPos;
in vec4 geoWorldPos;
in vec3 geoColor;
in vec3 geoNormal;
in vec3 geoGomo;

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
	float ambientStrength = 0.1f;
	ambientVec = ambientStrength * lightColor;			

    // diffuse 
    vec3 diffuseVec;
    vec3 lightDir = normalize(lightPos - geoWorldPos.xyz);

    float diff = max(dot(geoNormal, lightDir), 0.0f);
    diffuseVec = diff * lightColor;	

    //resulting light
    vec3 light = ambientVec + diffuseVec;

    //texel obtainment
    vec3 spherePos = normalize(geoModelPos.xyz);
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
    vec4 tex = texture(mapMain, texCoord);


	//edge
	if(abs(geoGomo[0]) < 0.02f || abs(geoGomo[1]) < 0.02f || abs(geoGomo[2]) < 0.02f)
	{
		tex = vec4(0.0);
	}

	//result
    FragColor = vec4(light * tex.xyz, 1.0f);
}