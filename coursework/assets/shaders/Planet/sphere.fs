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
    vec3 normalized = normalize(geoModelPos.xyz);
	
	float x = normalized.x;
	float y = normalized.y;
	float z = normalized.z;
	float r = sqrt(x * x + z * z);

	vec3 circle = vec3(x / r, 0, z / r);
	

    float latitude  = acos(y);
    float longitude = acos(circle.x);
    if (circle.z > 0.0)
    {
    	longitude = 2 * PI - longitude;
    }
    vec4 tex = texture(mapMain, vec2(longitude / PI_2, 1.0 - latitude / PI));

	
	if(abs(geoGomo[0]) < 0.02 || abs(geoGomo[1]) < 0.02 || abs(geoGomo[2]) < 0.02)
	{
		tex = vec4(vec3(0.0), 1.0);
	}


	//result
    FragColor = tex;
}