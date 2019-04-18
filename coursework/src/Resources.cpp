#include "Resources.h"


void createContext(GLFWwindow*& win, int width, int height, const String& name)
{
	auto glfwErr = glfwInit();

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	win = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(win);

	auto glewErr = glewInit();
}


//load shaders from assets
void loadShaders(std::map<String, ShaderShared>& shadersStorage)
{    
	shadersStorage["planet.vs"]  = ShaderShared(new Shader(Shader::Vertex, "assets/shaders/Planet/sphere.vs"));
	shadersStorage["planet.tcs"] = ShaderShared(new Shader(Shader::TessControl, "assets/shaders/Planet/sphere.tcs"));
	shadersStorage["planet.tes"] = ShaderShared(new Shader(Shader::TessEvaluation, "assets/shaders/Planet/sphere.tes"));
	shadersStorage["planet.gs"]  = ShaderShared(new Shader(Shader::Geometry, "assets/shaders/Planet/sphere.gs"));
	shadersStorage["planet.fs"]  = ShaderShared(new Shader(Shader::Fragment, "assets/shaders/Planet/sphere.fs"));

	shadersStorage["satellite.vs"] = ShaderShared(new Shader(Shader::Vertex, "assets/shaders/Satellite/satellite.vs"));
	shadersStorage["satellite.gs"] = ShaderShared(new Shader(Shader::Geometry, "assets/shaders/Satellite/satellite.gs"));
	shadersStorage["satellite.fs"] = ShaderShared(new Shader(Shader::Fragment, "assets/shaders/Satellite/satellite.fs"));

	shadersStorage["simple.vs"] = ShaderShared(new Shader(Shader::Vertex, "assets/shaders/Simple/simple.vs"));
	shadersStorage["simple.fs"] = ShaderShared(new Shader(Shader::Fragment, "assets/shaders/Simple/simple.fs"));

	for (const auto&[location, shader] : shadersStorage)
	{
		if (!(shader->compiled()))
		{
			std::cerr << "FAILED TO COMPILE SHADER: " << location << std::endl;
			std::cerr << shader->infoLog();
		}
	}
}


//load all models from assets + custom
void loadModels(std::map<String, ModelShared>& modelsStorage)
{
	AssimpBuilder assimpBuilder;
	//assimpBuilder.readFile("assets/textures/Satellite/10477_Satellite_v1_L3.obj");

	//modelsStorage["satellite"] = ModelShared(new Model(std::move(assimpBuilder.imported())));


	PlanetBuilder planetBuilder;
	planetBuilder.build(4, "assets/textures/earth/earthmap1k.jpg", "Earth");

	modelsStorage["earth"] = ModelShared(new Model(std::move(planetBuilder.model())));


	BoxBuilder boxBuilder;
	boxBuilder.build("box");

	modelsStorage["box"] = ModelShared(new Model(std::move(boxBuilder.model())));
}


//assemble shader programs
ShaderProgramShared createPlanetProgram(std::map<String, ShaderShared>& shadersStorage)
{
	ShaderProgramShared program = ShaderProgramShared(new ShaderProgram("Planet"));
	program->attachShader(*shadersStorage["planet.vs"]);
	program->attachShader(*shadersStorage["planet.tcs"]);
	program->attachShader(*shadersStorage["planet.tes"]);
	program->attachShader(*shadersStorage["planet.gs"]);
	program->attachShader(*shadersStorage["planet.fs"]);

	program->link();
	if (!(program->linked()))
	{
		std::cerr << program->infoLog();
	}

	return program;
}

ShaderProgramShared createSatelliteProgram(std::map<String, ShaderShared>& shadersStorage)
{
	ShaderProgramShared program = ShaderProgramShared(new ShaderProgram("Satellite"));
	program->attachShader(*shadersStorage["satellite.vs"]);
	program->attachShader(*shadersStorage["satellite.gs"]);
	program->attachShader(*shadersStorage["satellite.fs"]);

	program->link();
	if (!(program->linked()))
	{
		std::cerr << program->infoLog();
	}

	return program;
}

ShaderProgramShared createSimpleProgram(std::map<String, ShaderShared>& shadersHolder)
{
	ShaderProgramShared program = ShaderProgramShared(new ShaderProgram("Simple"));
	program->attachShader(*shadersHolder["simple.vs"]);
	program->attachShader(*shadersHolder["simple.fs"]);

	program->link();
	if (!(program->linked()))
	{
		std::cerr << program->infoLog();
	}

	return program;
}

void createShaderPrograms(
	std::map<String, ShaderProgramShared>& programStorage
	, std::map<String, ShaderShared>& shadersStorage
)
{
	programStorage["satellite"] = createSatelliteProgram(shadersStorage);
	programStorage["planet"]    = createPlanetProgram(shadersStorage);
	programStorage["simple"]    = createSimpleProgram(shadersStorage);
}


//assemble renderers
void createRenderers(
	std::map<String, IRendererShared>& renderersStorage
	, std::map<String, ShaderProgramShared>& programsStorage
)
{
	renderersStorage["satellite"] = IRendererShared(new SatelliteRenderer(programsStorage["satellite"]));
	renderersStorage["planet"]    = IRendererShared(new PlanetRenderer(programsStorage["planet"]));
	renderersStorage["simple"]    = IRendererShared(new SimpleRenderer(programsStorage["simple"]));
}
