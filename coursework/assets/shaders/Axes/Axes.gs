#version 430 core

#define SIZE 0.5

layout(points) in;
layout(line_strip, max_vertices = 6) out;


uniform dmat4 model;
uniform dmat4 view;
uniform dmat4 proj;


out vec4 gsColor;


dmat4 gramm_schmidt(dmat4 mat)
{
	dmat4 ortho = mat;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < i - 1; j++)
		{
			ortho[i] -= dot(ortho[i], ortho[j]) * ortho[j];
		}

		ortho[i] = normalize(ortho[i]);
	}

	return ortho;
}

void main()
{
	//orthogonolise & normalize axes vectors
	dmat4 axes = dmat4(model[0], model[1], model[2], dvec4(dvec3(0.0), 1.0)); // drop position vector
	axes = transpose(axes);
	axes = gramm_schmidt(axes);
	axes = transpose(axes);
	axes[3] = model[3];

	dmat4 mvp = proj * view * axes;

	vec4 center = SIZE * vec4(mvp * dvec4(0.0, 0.0, 0.0, 1.0));
	vec4 xAxis  = SIZE * vec4(mvp * dvec4(1.0, 0.0, 0.0, 1.0));
	vec4 yAxis  = SIZE * vec4(mvp * dvec4(0.0, 1.0, 0.0, 1.0));
	vec4 zAxis  = SIZE * vec4(mvp * dvec4(0.0, 0.0, 1.0, 1.0));
	
	//xAxis
	gsColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = center;
	EmitVertex();
	gsColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = xAxis;
	EmitVertex();
	EndPrimitive();

	//yAxis
	gsColor = vec4(0.0, 1.0, 0.0, 1.0);
	gl_Position = center;
	EmitVertex();
	gsColor = vec4(0.0, 1.0, 0.0, 1.0);
	gl_Position = yAxis;
	EmitVertex();
	EndPrimitive();

	//zAxis
	gsColor = vec4(0.0, 0.0, 1.0, 1.0);
	gl_Position = center;
	EmitVertex();
	gsColor = vec4(0.0, 0.0, 1.0, 1.0);
	gl_Position = zAxis;
	EmitVertex();
	EndPrimitive();
}