#pragma once

#include <Common.h>

#include <Shader/Shader.h>
#include <Shader/ShaderProgram.h>

#include <Model/Model.h>
#include <Model/VertexArrayBuffer.h>
#include <Model/Builders/AssimpBuilder.h>
#include <Model/Builders/CustomBuilders.h>

#include <Render/Renderers.h>

#include <Integrator/Integrators.h>


void createContext(GLFWwindow*& win, int width, int height, const String& name);


//load shaders from assets
void loadShaders(std::map<String, ShaderShared>& shadersStorage);


//load all models from assets + custom
void loadModels(std::map<String, ModelShared>& modelsStorage);


//assemble shader programs
ShaderProgramShared createPlanetProgram(std::map<String, ShaderShared>& shadersStorage);

ShaderProgramShared createSatelliteProgram(std::map<String, ShaderShared>& shadersStorage);

ShaderProgramShared createSimpleProgram(std::map<String, ShaderShared>& shadersHolder);

void createShaderPrograms(
	std::map<String, ShaderProgramShared>& programStorage
	, std::map<String, ShaderShared>& shadersStorage
);


//assemble renderers
void createRenderers(
	std::map<String, IRendererShared>& renderersStorage
	, std::map<String, ShaderProgramShared>& programsStorage
);


//create integrators
void createIntegrators(std::map<String, IntegratorShared>& integrators);