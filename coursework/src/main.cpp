#include <Objects/View.h>

#include <Resources.h>
#include <Factories.h>
#include <Numerics/Ivp/RungeKutta.h>

#include <GUI/GUI.h>


const int WIDTH  = 1500;
const int HEIGHT = 900;
 

//globals
//context
GLFWwindow* window;

//data
std::map<String, ShaderShared>        shaders;
std::map<String, ModelShared>         models;
std::map<String, ShaderProgramShared> programs;
std::map<String, IRendererShared>     renderers;
std::map<String, IntegratorShared>    integrators;

//Planet
PlanetShared earth;

//Satellites
SatelliteShared target;
SatelliteShared chaser;

//view
View view;

//gui state
SatelliteWeak currentWeak;
double timeTrack0;
double timeTrack1;

//flags
bool fill;          // wireframe mode
bool stopped;       // enable / disable objects update
bool scrollView;    // scroll view

//screen pos
double prevX;
double prevY;

//time stepping
Time t;
Time t0;
Time t1;
Time dt;
Time dt0;

uint64_t warp;
uint64_t warpMin;
uint64_t warpMax;

double divisor;


//callbacks(70)
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
            view.travelView(0.01 * R, View::Z);

            break;
        }
        case (GLFW_KEY_S):
        {
            view.travelView(-0.01 * R, View::Z);

            break;
        }
		case (GLFW_KEY_A):
		{
			view.travelView(0.01 * R, View::X);

			break;
		}
		case (GLFW_KEY_D):
		{
			view.travelView(-0.01 * R, View::X);

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
	if (scrollView)
	{
		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			view.rotateAxes((xPos - prevX) / WIDTH, View::Z);
		}
		else
		{
			view.lookAround(-(xPos - prevX) / WIDTH, -(yPos - prevY) / HEIGHT);
		}
	}

    prevX = xPos;
    prevY = yPos;
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		scrollView = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		scrollView = false;
	}
}


//init all globals(100)
void initGlobals()
{
	//context
	std::cout << "---Context---" << std::endl;
	std::cout << "Creating context..." << std::endl;
	createContext(window, WIDTH, HEIGHT, "Rendezvous");
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, posCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
	std::cout << "Context created" << std::endl;
	std::cout << std::endl;

	//resources
	std::cout << "---Resources---" << std::endl;
	std::cout << "Shaders..." << std::endl;
	loadShaders(shaders);

	std::cout << "Models..." << std::endl;
	loadModels(models);

	std::cout << "Programs..." << std::endl;
	createShaderPrograms(programs, shaders);

	std::cout << "Renderers..." << std::endl;
	createRenderers(renderers, programs);

	std::cout << "Integrators..." << std::endl;
	createIntegrators(integrators);
	std::cout << std::endl;

	//view
	Vec3 pos  = Vec3(0.0, 1.5 * R, 0.0);
	Vec3 look = Vec3(0.0);
	Vec3 up   = Vec3(0.0, 0.0, 1.0);
	view = View(
		  glm::lookAt(pos, look, up)
		, glm::perspective(glm::radians(45.0), 1.0 * WIDTH / HEIGHT, 1.0, 2 * R)
		, pos
		, 0.5
	);

	//planet
	earth = createPlanet(
		models["earth"]
		, M
		, glm::scale(Mat4(1.0), Vec3(R))
		, Vec3(0.0)
		, Vec3(0.0)
		, Vec3(0.0)
		, "Earth"
	);

	//target
	target = createSatellite(
		models["satellite"]
		, 1.0
		, Vec3(1.0, 0.0, 0.0)
		, glm::scale(Mat4(1.0), Vec3(3.0))
		, Vec3(R + 4e+5, 0.0, 500.0)
		, Vec3(0.0, 0.0, sqrt(G * M / (R + 4e+5)))
		, "target"
		, earth->mPhysics
	);

	//chaser
	chaser = createSatellite(
		models["satellite"]
		, 1.0
		, Vec3(1.0, 0.0, 1.0)
		, glm::scale(Mat4(1.0), Vec3(3.0))
		, Vec3(R + 4e+5, 0.0, 0.0)
		, Vec3(0.0, 0.0, -sqrt(G * M / (R + 4e+5)))
		, "chaser"
		, earth->mPhysics
	);

	//render lists
	renderers["satellite"]->addToList(target);
	renderers["satellite"]->addToList(chaser);
	renderers["planet"]->addToList(earth);

	//gui
	currentWeak = SatelliteShared(nullptr);
	timeTrack0 = 0.0f;
	timeTrack1 = 0.0f;

	//loop states
	fill    = false;
	stopped = true;
	scrollView = false;

	prevX = WIDTH / 2;
	prevY = HEIGHT / 2;

	//time step
	divisor = (double)glfwGetTimerFrequency();
	t  = Time();
	t0 = Time();
	t1 = Time();
	dt = Time();
	dt0 = Time(50, 50 / divisor);

	warp    = 1;
	warpMin = 1;
	warpMax = 1000000;
}

void destroyGlobals()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}


//main loop

//integrator(3 lines)
void updateSatPlanet(SatelliteShared& sat, PlanetShared& planet, const Time& t, const Time& dt)
{	
	integrators["gaussLegendre6"]->updatePhysics(planet, sat, t, dt);
}

void updateObjects()
{
	earth->update(t, dt);

	target->update(t, dt);
	chaser->update(t, dt);
}

void updatePhysics()
{
	updateSatPlanet(target, earth, t, dt);
	updateSatPlanet(chaser, earth, t, dt);
}

void updateTime()
{
	uint64_t raw = glfwGetTimerValue();
	t1 = Time(raw, raw / divisor);

	uint64_t delta = raw - t0.asU64();
	dt += Time(delta * warp);

	if (dt > dt0 * (warp))
	{
		dt = dt0 * (warp);
	}

	uint64_t time = t.asU64() + dt.asU64();
	t  = Time(time, time / divisor);
	dt = Time(dt.asU64(), dt.asU64() / divisor);

	t0 = t1;
}

void update()
{
	updateTime();

	updatePhysics();
	updateObjects();
}


//gui(200 lines)
//test gui
void testGui()
{
	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


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
}

//my gui
void systemOptions()
{
	if (ImGui::CollapsingHeader("System parameters"))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("");

		ImGui::Text("Time raw: %llu", t.asU64()  );
		ImGui::Text("Time    : %f"  , t.asFloat());
		ImGui::Text("Divisor : %f", divisor);

		ImGui::SliderScalar("Time warp: ", ImGuiDataType_S64, &warp, &warpMin, &warpMax);
		warp = std::min(warp, warpMax);
		warp = std::max(warp, warpMin);

		static const char* options[] = {"pause simulation", "resume simulation"};
		if (ImGui::Button(options[stopped]))
		{
			stopped ^= true;
		}
		ImGui::Text("");

		


	}
}

void planetOptions()
{
	if (ImGui::CollapsingHeader("Planet parameters"))
	{
		ImGui::Text("Not implemented yet");
	}
}

void showSatelliteParameters(SatelliteShared& sat)
{
	if (sat->mName)
	{
		auto& name = *(sat->mName);

		ImGui::Text("Name: %s", name.mName.c_str());
		ImGui::Text("");
	}

	if (sat->mPhysics)
	{
		auto& physics = *(sat->mPhysics);

		ImGui::Text("State parameters");
		ImGui::Text("Radius   x:%f y:%f z:%f", physics.mPosition.x, physics.mPosition.y, physics.mPosition.z);
		ImGui::Text("Velocity x:%f y:%f z:%f", physics.mVelocity.x, physics.mVelocity.y, physics.mVelocity.z);
		ImGui::Text("");
	}

	if (sat->mOrbit)
	{
		auto& orbit = *(sat->mOrbit);

		ImGui::Text("Main");
		ImGui::Text("Specific ang. momentum : %f", orbit.angularMomentum());
		ImGui::Text("Right ascension        : %f", orbit.rightAscension());
		ImGui::Text("Argument of perigee    : %f", orbit.argumentOfPeriapsis());
		ImGui::Text("Inclination            : %f", orbit.inclination());
		ImGui::Text("Eccentricity           : %f", orbit.eccentricity());
		ImGui::Text("True anomaly           : %f", orbit.trueAnomaly());
		ImGui::Text("");	

		ImGui::Text("Spec ");
		ImGui::Text("Eccentric anomaly : %f", orbit.eccentricAnomaly());
		ImGui::Text("Apoapsis          : %f", orbit.apoapsis());
		ImGui::Text("Orbit period      : %f", orbit.period());
		ImGui::Text("Time              : %f", orbit.time());
		ImGui::Text("");
	}
}

void satellitesOptions()
{
	if (ImGui::CollapsingHeader("Target"))
	{
		showSatelliteParameters(target);
	}
	if (ImGui::CollapsingHeader("Chaser"))
	{
		showSatelliteParameters(chaser);
	}
}

void rendezvousOptions()
{
	if (ImGui::CollapsingHeader("Rendezvous"))
	{
		ImGui::Text("Not implemented yet");
	}
}

void myGui()
{
	ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints({200, 400}, {400, 800}, nullptr, nullptr);

	ImGui::Begin("Options", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoResize| ImGuiWindowFlags_AlwaysAutoResize);
	
	systemOptions();

	planetOptions();

	satellitesOptions();

	rendezvousOptions();

	ImGui::End();
}


//gui init/render/destroy
void initGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");

}

void renderGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	testGui();

	myGui();


	ImGui::Render();
	glClear(GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void destroyGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void render()
{
	for (auto&[name, renderer] : renderers)
	{
		renderer->render(view);
	}
}


void featureTest()
{
	initGlobals();
	initGui();


    //setups
	glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	
	glfwSetWindowPos(window, mode->width / 2 - WIDTH / 2, mode->height / 2 - HEIGHT / 2	);
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);
    glfwSetCursorPos(window, prevX, prevY);
	glfwSwapInterval(1);
	
	view.setTrack(target->mPhysics, Vec3(20.0, 0.0, 0.0));
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();


		if (!stopped)
		{
			update();
		}
		view.update(t, dt);

		render();
		renderGui();

		glfwSwapBuffers(window);
    }

	destroyGui();
	destroyGlobals();
}

 

int main(int argc, char* argv[])
{
    featureTest();
	
    return EXIT_SUCCESS;
}