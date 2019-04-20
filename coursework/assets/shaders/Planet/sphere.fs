#version 430 core

flat in dvec4 geoModelPos[3];
flat in dvec4 geoWorldPos[3];
flat in dvec3 geoColor[3];
flat in dvec3 geoNormal[3];
in vec3 geoGomo;

out vec4 FragColor;


uniform sampler2D mapMain;


const double PI   = acos(-1.0); 
const double PI_2 = 2 * PI;


dvec4 gomo(dvec4 vecs[3], vec3 gomo)
{
	return vecs[0] * gomo[0] + vecs[1] * gomo[1] + vecs[2] * gomo[2];
}

dvec3 gomo(dvec3 vecs[3], vec3 gomo)
{
	return vecs[0] * gomo[0] + vecs[1] * gomo[1] + vecs[2] * gomo[2];
}

void main()
{
	dvec4 modelPos = gomo(geoModelPos, geoGomo);
	dvec4 worldPos = gomo(geoWorldPos, geoGomo);
	dvec3 color    = gomo(geoColor   , geoGomo);
	dvec3 normal   = gomo(geoNormal  , geoGomo);

    //texel obtainment
    dvec3 normalized = normalize(modelPos.xyz);
	
	double x = normalized.x;
	double y = normalized.y;
	double z = normalized.z;
	double r = sqrt(x * x + z * z);

	dvec3 circle = dvec3(x / r, 0.0, z / r);
	

    double latitude  = acos(float(y));
    double longitude = acos(float(circle.x));
    if (circle.z > 0.0)
    {
    	longitude = (2 * PI - longitude);
    }
    vec4 tex = texture(mapMain, vec2(longitude / PI_2, 1.0 - latitude / PI));

	//if(any(bvec3(geoGomo[0] < 0.02, geoGomo[1] < 0.02, geoGomo[2] < 0.02)))
	//{
	//	tex = vec4(vec3(0.0), 1.0);
	//}


	//result
    FragColor = tex;
}