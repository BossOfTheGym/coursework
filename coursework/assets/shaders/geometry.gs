#version 430 core

#define MAX_SPLIT 8


layout (triangles) in;
layout (triangle_strip, max_vertices = 192) out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


in vec3 ourColor[];

out vec3 outColor;


struct Vertex
{
	vec4 pos;
	vec3 color;
};


void assembleTriangle(
	vec4 vertex0, vec3 color0
	, vec4 vertex1, vec3 color1
	, vec4 vertex2, vec3 color2
)
{
	gl_Position = vertex0;
	outColor    = color0;
	EmitVertex();

	gl_Position = vertex1;
	outColor    = color1;
	EmitVertex();

	gl_Position = vertex2;
	outColor    = color2;
	EmitVertex();

	EndPrimitive();
}


void splitTriangle(int maxSplit)
{
	//buffer for vertices 
	Vertex vertexBuffer[MAX_SPLIT + 1][MAX_SPLIT + 1];
	
	//lambda(gomogenious coordinates) stride
	float delta = 1.0f / maxSplit;
	
	//gl_in vertices & colors
	vec3 vertex0 = gl_in[0].gl_Position.xyz;
	vec3 vertex1 = gl_in[1].gl_Position.xyz;
	vec3 vertex2 = gl_in[2].gl_Position.xyz;

	vec3 color0 = ourColor[0];
	vec3 color1 = ourColor[1];
	vec3 color2 = ourColor[2];


	//initialize buffer
	for(int i = 0; i <= maxSplit; i++)
	{
		for(int j = 0 ; j <= maxSplit - i; j++)
		{
			//barycentric coordinates
			float lambda0 = i * delta;
			float lambda1 = j * delta;
			float lambda2 = 1.0f - lambda1 - lambda0;

			vec3 newVertex = lambda0 * vertex0 + lambda1 * vertex1 + lambda2 * vertex2;
			vec3 newColor  = lambda0 * color0  + lambda1 * color1  + lambda2 * color2;


			vertexBuffer[i][j].pos   = projection * view * model * vec4(normalize(newVertex), 1.0);
			vertexBuffer[i][j].color = newColor;
		}
	}


	//assemble triangles
	for(int i = 0; i < maxSplit; i++)
	{
		for(int j = 0; j < maxSplit - i - 1; j++)
		{
			assembleTriangle(
				  vertexBuffer[  i  ][  j  ].pos, vertexBuffer[  i  ][  j  ].color
				, vertexBuffer[  i  ][j + 1].pos, vertexBuffer[  i  ][j + 1].color
				, vertexBuffer[i + 1][  j  ].pos, vertexBuffer[i + 1][  j  ].color
			);

			assembleTriangle(
				  vertexBuffer[i + 1][  j  ].pos, vertexBuffer[i + 1][  j  ].color
				, vertexBuffer[i + 1][j + 1].pos, vertexBuffer[i + 1][j + 1].color
				, vertexBuffer[  i  ][j + 1].pos, vertexBuffer[  i  ][j + 1].color
			);
		}

		assembleTriangle(
			  vertexBuffer[  i  ][maxSplit - i - 1].pos, vertexBuffer[  i  ][maxSplit - i - 1].color
			, vertexBuffer[  i  ][maxSplit - i    ].pos, vertexBuffer[  i  ][maxSplit - i    ].color
			, vertexBuffer[i + 1][maxSplit - i - 1].pos, vertexBuffer[i + 1][maxSplit - i - 1].color
		);
	}
}


void main()
{
	splitTriangle(8);
}
