#include "Resources.h"


void initContext(int width, int height, const String& name)
{}


//load shaders from assets
void loadShaders(std::map<String, ShaderShared>& shadersStorage)
{    
	shadersStorage["planet.vs"]  = ShaderShared(new Shader(Shader::Vertex, "assets/shaders/Planet/sphere.vs"));
	shadersStorage["planet.gs"]  = ShaderShared(new Shader(Shader::Geometry, "assets/shaders/Planet/sphere.gs"));
	shadersStorage["planet.fs"]  = ShaderShared(new Shader(Shader::Fragment, "assets/shaders/Planet/sphere.fs"));

	shadersStorage["satellite.vs"] = ShaderShared(new Shader(Shader::Vertex, "assets/shaders/Satellite/satellite.vs"));
	shadersStorage["satellite.gs"] = ShaderShared(new Shader(Shader::Geometry, "assets/shaders/Satellite/satellite.gs"));
	shadersStorage["satellite.fs"] = ShaderShared(new Shader(Shader::Fragment, "assets/shaders/Satellite/satellite.fs"));

	shadersStorage["simple.vs"] = ShaderShared(new Shader(Shader::Vertex, "assets/shaders/Simple/simple.vs"));
	shadersStorage["simple.fs"] = ShaderShared(new Shader(Shader::Fragment, "assets/shaders/Simple/simple.fs"));

	shadersStorage["axes.vs"] = ShaderShared(new Shader(Shader::Vertex, "assets/shaders/Axes/axes.vs"));
	shadersStorage["axes.gs"] = ShaderShared(new Shader(Shader::Geometry, "assets/shaders/Axes/axes.gs"));
	shadersStorage["axes.fs"] = ShaderShared(new Shader(Shader::Fragment, "assets/shaders/Axes/axes.fs"));

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
	assimpBuilder.readFile("assets/textures/Satellite/10477_Satellite_v1_L3.obj");

	modelsStorage["satellite"] = ModelShared(new Model(std::move(assimpBuilder.imported())));

	
	PlanetBuilder planetBuilder;
	planetBuilder.build(6, "assets/textures/earth/earthmap.jpg", "Earth");

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

ShaderProgramShared createAxesProgram(std::map<String, ShaderShared>& shadersHolder)
{
	ShaderProgramShared program = ShaderProgramShared(new ShaderProgram("Axes"));
	program->attachShader(*shadersHolder["axes.vs"]);
	program->attachShader(*shadersHolder["axes.gs"]);
	program->attachShader(*shadersHolder["axes.fs"]);

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
	programStorage["axes"]      = createAxesProgram(shadersStorage);
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


void createIntegrators(std::map<String, IntegratorShared>& integrators)
{
	integrators["classic4"]       = IntegratorShared(new RungeKuttaExplicit4());

	integrators["gaussLegendre6"] = IntegratorShared(new RungeKuttaImplicit6(1e-15, 30));
}