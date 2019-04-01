#include <Objects/View.h>

#include <Shader/Shader.h>
#include <Shader/ShaderProgram.h>

#include <Model/Model.h>
#include <Model/VertexArrayBuffer.h>
#include <Model/Builders/AssimpBuilder.h>
#include <Model/Builders/CustomBuilders.h>

#include <Components.h>
#include <Objects/Objects.h>
#include <Render/Renderers.h>


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>



const int WIDTH  = 1200;
const int HEIGHT = 900;
 

using namespace std::chrono;


//create context for OpenGL and input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void posCallback(GLFWwindow* window, double xPos, double yPos);
void errorCallback(int error, const char* description);

GLFWwindow* createWindow()
{
    auto glfwErr = glfwInit();

    GLFWwindow* win;

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    win = glfwCreateWindow(WIDTH, HEIGHT, "PINGAS PROD", nullptr, nullptr);

    glfwSetKeyCallback(win, keyCallback);
    glfwSetCursorPosCallback(win, posCallback);
    glfwSetErrorCallback(errorCallback);

    glfwMakeContextCurrent(win);

    auto glewErr = glewInit();

    return win;
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
	planetBuilder.build(3, "assets/textures/earth/earthmap1k.jpg", "Earth");

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



//object factories
IObjectShared createSatellite(
	  const ModelShared& model
	, const Vec3& color = Vec3(1.0f)
	, const Mat4& mat = Mat4(1.0f)
	, const Vec3& pos = Vec3()
	, const Vec3& vel = Vec3()
)
{
	SatelliteShared satellite = std::make_shared<Satellite>();


	satellite->mPhysics  = PhysicsComponentShared(
		new PhysicsComponent(nullptr, mat, pos, vel, Vec3())
	);

	satellite->mGraphics = GraphicsComponentShared(
		new GraphicsComponent(
			nullptr
			, model
			, satellite->mPhysics
		)
	);

	satellite->mSatellite = SatelliteComponentShared(
		new SatelliteComponent(nullptr, color)
	);


	return satellite;
}

IObjectShared createPlanet(
	  const ModelShared& model
	, float mass
	, const Mat4& mat = Mat4(1.0f)
	, const Vec3& pos = Vec3()
	, const Vec3& vel = Vec3()
	, const Vec3& angularMomentum = Vec3()
)
{
	PlanetShared planet = std::make_shared<Planet>();


	planet->mPhysics  = PhysicsComponentShared(
		new PhysicsComponent(nullptr, mat, pos, vel, angularMomentum)
		);
	planet->mPhysics->mass = mass;

	planet->mGraphics = GraphicsComponentShared(
		new GraphicsComponent(
			nullptr
			, model
			, planet->mPhysics
		)
	);


	return planet;
}


//globals
//context
GLFWwindow* window;

//objects
std::map<String, ShaderShared>        shaders;
std::map<String, ModelShared>         models;
std::map<String, ShaderProgramShared> programs;
std::map<String, IRendererShared>     renderers;
std::map<String, IObjectShared>       objects;

//cached
SatelliteShared sat1;
SatelliteShared sat2;
PlanetShared earth;

//view
View view;

//flags
bool fill;
bool stopped;

//screen pos
double prevX;
double prevY;

//time stepping
uint64_t t0;
uint64_t t1;
uint64_t delta;


//main
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
        case (GLFW_KEY_C):
        {
            if (action == GLFW_PRESS)
            {
                fill ^= true;
                glPolygonMode(GL_FRONT_AND_BACK, (fill ? GL_FILL : GL_LINE));
            }

            break;
        }
        case (GLFW_KEY_W):
        {

            view.travelView(0.1f, View::Z);

            break;
        }
        case (GLFW_KEY_S):
        {
            view.travelView(-0.1f, View::Z);

            break;
        }
		case (GLFW_KEY_A):
		{
			view.travelView(0.1f, View::X);

			break;
		}
		case (GLFW_KEY_D):
		{
			view.travelView(-0.1f, View::X);

			break;
		}
        case (GLFW_KEY_R):
        {
            prevX = WIDTH / 2;
            prevY = HEIGHT / 2;
            glfwSetCursorPos(window, prevX, prevY);

            break;
        }
    }
}

void posCallback(GLFWwindow* window, double xPos, double yPos)
{
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		view.rotateAxes((xPos - prevX) / WIDTH, View::Z);
	}
	else
	{
		view.lookAround((xPos - prevX) / WIDTH, (yPos - prevY) / HEIGHT);
	}

	/*View::Axis axis1;
	View::Axis axis2;

	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		axis1 = View::Z;
		axis2 = View::X;
	}
	else
	{
		axis1 = View::Y;
		axis2 = View::X;
	}

	view.rotateView(static_cast<float>(xPos - prevX) / WIDTH, axis1);
	view.rotateView(static_cast<float>(yPos - prevY) / HEIGHT, axis2);*/
    prevX = xPos;
    prevY = yPos;
}

void errorCallback(int error, const char* msg)
{
    std::cout << "Error callback" << std::endl;

    std::cout << "ERROR: " << error << std::endl;
    std::cout << "MSG: " << msg << std::endl;
}


void initGlobals()
{
	window  = std::move(createWindow());
	

	loadShaders(shaders);
	loadModels(models);
	createShaderPrograms(programs, shaders);
	createRenderers(renderers, programs);
	

	Vec3 pos  = Vec3(0.0f, 10.0f, 0.0f);
	Vec3 look = Vec3(0.0f);
	Vec3 up   = Vec3(0.0f, 0.0f, 1.0f);
	view = View(
		  glm::lookAt(pos, look, up)
		, glm::perspective(glm::radians(45.0f), 1.0f * WIDTH / HEIGHT, 0.1f, 500.0f)
		, pos
	);


	sat1 = std::dynamic_pointer_cast<Satellite>(
		createSatellite(
			  models["box"]
			, Vec3(1.0f, 0.0f, 1.0f)
			, glm::scale(Mat4(1.0f), Vec3(0.2f))
			, Vec3(-6.0f, 0.0f, 0.0f)
			, Vec3(0.0f, 0.0f, 18.0f)
		)
	);
	sat2 = std::dynamic_pointer_cast<Satellite>(
		createSatellite(
			  models["box"]
			, Vec3(1.0f, 0.0f, 1.0f)
			, glm::scale(Mat4(1.0f), Vec3(0.2f))
			, Vec3(6.0f, 0.0f, 0.0f)
			, Vec3(0.0f, 0.0f, -18.0f)
		)
	);
	earth = std::dynamic_pointer_cast<Planet>(
		createPlanet(
			  models["earth"]
			, 2000.0f
			, glm::scale(Mat4(1.0f), Vec3(2.0f))
			, Vec3(0.0f)
			, Vec3(0.0f)
			, Vec3(0.0f)
		)
	);


	fill    = false;
	stopped = true;

	prevX = WIDTH / 2;
	prevY = HEIGHT / 2;

	t0 = glfwGetTimerValue();
	t1 = 0;
	delta = 0;
}



//main loop
void updateSatPlanet(SatelliteShared& sat, PlanetShared& planet, uint64_t time_step)
{
	auto& mat = sat->mPhysics->mMat;
	auto& r   = sat->mPhysics->mPosition;
	auto& v   = sat->mPhysics->mVelocity;

	auto& r0 = planet->mPhysics->mPosition;
	auto& M  = planet->mPhysics->mass;


	auto dr   = r - r0;
	auto temp = v;

	auto dt = time_step / 1000000.0f;

	r += dt * temp;
	v += dt * (-M / glm::dot(dr, dr)) * glm::normalize(dr);


	mat[3] = Vec4(r, 1.0f);
}

void updatePhysics()
{
	static const uint64_t time_step = 10;
	static const uint64_t max_count = 5;
	
	t1 = glfwGetTimerValue();
	delta += (t1 - t0) / 30;
	
	if (delta > max_count * time_step)
	{
		delta = max_count * time_step;
	}

	while (delta >= time_step)
	{
		delta -= time_step;

		updateSatPlanet(sat1, earth, time_step);
		updateSatPlanet(sat2, earth, time_step);
	}

	t0 = t1;
}



void initGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
}

void testGui()
{
	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);


	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	ImGui::Render();
	glClear(GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void systemOptions()
{

}

void planetOptions()
{
	//rotation
}

void satelliteOptions()
{

}

void myGui()
{

}

void renderGui()
{
	testGui();
}

void destroyGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void render()
{
	glfwMakeContextCurrent(window);

	auto& satellite = renderers["satellite"];
	satellite->setRequiredStates();
	satellite->renderComponent(sat1->mGraphics, view);
	satellite->renderComponent(sat2->mGraphics, view);

	auto& planet = renderers["planet"];
	planet->setRequiredStates();
	planet->renderComponent(earth->mGraphics, view);
}


void featureTest()
{
	initGlobals();
	initGui();


    //setups
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	

	glfwShowWindow(window);
	glfwMakeContextCurrent(window);
    glfwSetCursorPos(window, prevX, prevY);
	t0 = glfwGetTimerValue();
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();

		if (!stopped)
		{
			updatePhysics();
		}

		render();
		renderGui();

		glfwSwapBuffers(window);
    }

	destroyGui();
    glfwDestroyWindow(window);
    glfwTerminate();
}

 

int main(int argc, char* argv[])
{
    featureTest();

    return EXIT_SUCCESS;
}