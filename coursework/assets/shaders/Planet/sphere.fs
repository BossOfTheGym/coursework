#version 430 core

in vec4 geoModelPos;
in vec4 geoWorldPos;
in vec3 geoColor;
in vec3 geoNormal;
in vec3 geoGomo;

out vec4 FragColor;


uniform sampler2D mapMain;


const float PI = acos(-1.0f); 
const float PI_2 = 2 * PI;


void main()
{
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
	//if(abs(geoGomo[0]) < 0.02f || abs(geoGomo[1]) < 0.02f || abs(geoGomo[2]) < 0.02f)
	//{
	//	tex = vec4(0.0);
	//}

	//result
    FragColor = tex;
}