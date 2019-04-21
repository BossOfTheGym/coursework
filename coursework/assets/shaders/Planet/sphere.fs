#version 430 core

flat in dvec4 geoModelPos[3];
flat in dvec4 geoWorldPos[3];
flat in dvec3 geoColor[3];
flat in dvec3 geoNormal[3];
in vec3 geoGomo;

out vec4 FragColor;


uniform sampler2D mapMain;


const float PI   = 3.14159265358979; 
const float PI_2 = 2 * PI;


vec4 gomo(dvec4 vecs[3], vec3 gomo)
{
	return vec4(vecs[0] * gomo[0] + vecs[1] * gomo[1] + vecs[2] * gomo[2]);
}

vec3 gomo(dvec3 vecs[3], vec3 gomo)
{
	return vec3(vecs[0] * gomo[0] + vecs[1] * gomo[1] + vecs[2] * gomo[2]);
}

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
	vec4 modelPos = gomo(geoModelPos, geoGomo);
	vec4 worldPos = gomo(geoWorldPos, geoGomo);
	vec3 color    = gomo(geoColor   , geoGomo);
	vec3 normal   = gomo(geoNormal  , geoGomo);

    //texel obtainment
    vec3 normalized = normalize(modelPos.xyz);
	
	float x = normalized.x;
	float y = normalized.y;
	float z = normalized.z;
	float r = sqrt(x * x + z * z);
	
    float latitude  = acosNV(y);
    float longitude = acosNV(x / r);
    if (z / r > 0.0)
    {
    	longitude = (2 * PI - longitude);
    }
    vec4 tex = texture(mapMain, vec2(longitude / PI_2, 1.0 - latitude / PI));

	if(geoGomo[0] < 0.04 || geoGomo[1] < 0.04 || geoGomo[2] < 0.04)
	{
		tex = vec4(vec3(0.0), 1.0);
	}


	//result
    FragColor = tex;
}