#version 430 core

in vec4 geoModelPos;
in vec4 geoWorldPos;
in vec3 geoColor;
in vec3 geoNormal;
in vec3 geoGomo;

out vec4 FragColor;


uniform sampler2D mapMain;


const float PI   = 3.14159265358979; 
const float PI_2 = 2 * PI;


float acosNV(float x) {
	float negate = float(x < 0);
	x = abs(x);
	float ret = -0.0187293;
	ret = ret * x;
	ret = ret + 0.0742610;
	ret = ret * x;
	ret = ret - 0.2121144;
	ret = ret * x;
	ret = ret + 1.5707288;
	ret = ret * sqrt(1.0-x);
	ret = ret - 2 * negate * ret;
	return negate * 3.14159265358979 + ret;
}


void main()
{
    //texel obtainment
    vec3 normalized = normalize(geoModelPos.xyz);
	
	float x = normalized.x;
	float y = normalized.y;
	float z = normalized.z;
	float r = sqrt(x * x + z * z);
	
    float latitude  = acos(y);
    float longitude = acos(x / r);
    if (z / r > 0.0)
    {
    	longitude = (2 * PI - longitude);
    }
    vec4 tex = texture(mapMain, vec2(longitude / PI_2, 1.0 - latitude / PI));

	float d = min(min(geoGomo[0], geoGomo[1]), geoGomo[2]);
	if(d < 0.02)
	{
		//tex = vec4(vec3(0.0), 1.0);
	}


	//result
    FragColor = tex;
}