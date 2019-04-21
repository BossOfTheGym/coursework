#include <Objects/View.h>

#include <Resources.h>
#include <Factories.h>
#include <Numerics/Ivp/RungeKutta.h>
#include <Integrators/Integrator.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

using namespace std::chrono;


const int WIDTH  = 1800;
const int HEIGHT = 1000;
 

//globals
//context
GLFWwindow* window;

//objects
std::map<String, ShaderShared>        shaders;
std::map<String, ModelShared>         models;
std::map<String, ShaderProgramShared> programs;
std::map<String, IRendererShared>     renderers;
std::map<String, SatelliteShared>     satellites;
PlanetShared earth;

//view
View view;

//gui state
SatelliteWeak currentWeak;
double timeTrack0;
double timeTrack1;

//flags
bool fill;          // wireframe mode
bool stopped;       // enable / disable physics update
bool menuOpened;    // if opened disable input

//screen pos
double prevX;
double prevY;

//time stepping
Time t;
Time t0;
Time t1;
Time dt;
Time dt0;

uint64_t maxUpdates;

uint64_t warp;
uint64_t warpMin;
uint64_t warpMax;

double divisor;

//test
ShaderProgramShared axes;
GLint uModel;
GLint uView;
GLint uProj;


//callbacks
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(menuOpened)
	{
		return;
	}

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
	if (menuOpened)
	{
		return;
	}

	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		view.rotateAxes((xPos - prevX) / WIDTH, View::Z);
	}
	else
	{
		view.lookAround((xPos - prevX) / WIDTH, (yPos - prevY) / HEIGHT);
	}

    prevX = xPos;
    prevY = yPos;
}

//init all globals
void initGlobals()
{
	//context
	std::cout << "---Context---" << std::endl;
	std::cout << "Creating context..." << std::endl;
	createContext(window, WIDTH, HEIGHT, "Rendezvous");
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, posCallback);
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
	std::cout << std::endl;

	//view
	Vec3 pos  = Vec3(0.0f, 30.0f, 0.0f);
	Vec3 look = Vec3(0.0f);
	Vec3 up   = Vec3(0.0f, 0.0f, 1.0f);
	view = View(
		  glm::lookAt(pos, look, up)
		, glm::perspective(glm::radians(45.0f), 1.0f * WIDTH / HEIGHT, 0.1f, 500.0f)
		, pos
	);

	//planet
	earth = createPlanet(
		models["earth"]
		, 3000.0f
		, glm::scale(Mat4(1.0f), Vec3(4.0f))
		, Vec3(0.0f)
		, Vec3(0.0f)
		, Vec3(0.0f)
	);

	//satellites
	satellites["satellite 1"] = createSatellite(
		models["satellite"]
		, 1.0f
		, Vec3(1.0f, 0.0f, 0.0f)
		, glm::scale(Mat4(1.0f), Vec3(0.5f))
		, Vec3(-5.0f, 0.0f, 0.0f)
		, Vec3(0.0f, 0.0f, sqrt(3000.0 / 5))
		, "target 1"
		, earth->mPhysics
	);

	satellites["satellite 2"] = createSatellite(
		models["satellite"]
		, 1.0f
		, Vec3(1.0f, 0.0f, 0.0f)
		, glm::scale(Mat4(1.0f), Vec3(0.5f))
		, Vec3(-6.0f, 0.0f, 0.0f)
		, Vec3(0.0f, 0.0f, sqrt(3000.0 / 6))
		, "target 2"
		, earth->mPhysics
	);

	satellites["satellite 3"] = createSatellite(
		models["satellite"]
		, 1.0f
		, Vec3(1.0f, 0.0f, 0.0f)
		, glm::scale(Mat4(1.0f), Vec3(0.5f))
		, Vec3(-7.0f, 0.0f, 0.0f)
		, Vec3(0.0f, 0.0f, sqrt(3000.0 / 7))
		, "target 3"
		, earth->mPhysics
	);

	satellites["satellite 0"] = createChaser(
		models["satellite"]
		, 1.0f
		, Vec3(1.0f, 0.0f, 1.0f)
		, glm::scale(Mat4(1.0f), Vec3(0.5f))
		, Vec3(-5.0f, 0.0f, 0.0f)
		, Vec3(0.0f, 0.0f, 30.0)
		, "chaser"
		, earth->mPhysics
	);

	//render lists
	for (auto&[name, sat] : satellites)
	{
		renderers["satellite"]->addToList(sat);
	}
	renderers["planet"]->addToList(earth);

	//gui
	currentWeak = SatelliteShared(nullptr);
	timeTrack0 = 0.0f;
	timeTrack1 = 0.0f;

	//loop states
	fill    = false;
	stopped = false;
	menuOpened = false;

	prevX = WIDTH / 2;
	prevY = HEIGHT / 2;

	//time step
	divisor = (double)glfwGetTimerFrequency();
	t  = Time();
	t0 = Time();
	t1 = Time();
	dt = Time();
	dt0 = Time(40, 40 / divisor);
	maxUpdates = 5;	

	warp    = 1;
	warpMin = 1;
	warpMax = 1000;

	//test
	axes = programs["axes"];
	uModel = axes->getUniformLocation("model");
	uView  = axes->getUniformLocation("view");
	uProj  = axes->getUniformLocation("proj");
}

void destroyGlobals()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}


//main loop

//integrator
void updateSatPlanet(SatelliteShared& sat, PlanetShared& planet, const Time& t, const Time& dt)
{
	using glm::dot;
	using glm::length;

	using StateVec = Num::Arg::VecN<double, 6>;
	using Methods  = Num::Ivp::Methods;
	using Solver   = Num::Ivp::RungeKuttaExplicit<6, double, StateVec>;

	auto& mat = sat->mPhysics->mMat;
	auto& r   = sat->mPhysics->mPosition;
	auto& v   = sat->mPhysics->mVelocity;

	auto& r0 = planet->mPhysics->mPosition;
	auto  mu = G * planet->mPhysics->mMass;

	auto dr = r - r0;
	 

	Solver solver;
	auto force = [&] (double t, const StateVec& vec) -> StateVec
	{
		Vec3 rv = Vec3(vec[0], vec[1], vec[2]);
		Vec3 vv = Vec3(vec[3], vec[4], vec[5]);

		auto r = length(rv);

		Vec3 newV = -mu / (r * r) * (rv / r);

		return StateVec{vv[0], vv[1], vv[2], newV[0], newV[1], newV[2]};
	};

	StateVec curr{dr[0], dr[1], dr[2], v[0], v[1], v[2]};
	StateVec next = solver.solve(
		  force
		, Methods::classic4<double>
		, t.asFloat()
		, curr
		, dt.asFloat()
	).second;


	//update
	r = Vec3(next[0] + r0[0], next[1] + r0[1], next[2] + r0[2]);
	v = Vec3(next[3], next[4], next[5]);
	mat[3] = Vec4(r, 1.0f);
}

void updateObjects()
{
	earth->update(t, dt);

	for (auto&[key, sat] : satellites)
	{
		sat->update(t, dt);
	}
}

void updatePhysics()
{
	while (dt >= dt0 * warp)
	{
		dt -= dt0 * warp;

		for (auto&[key, sat] : satellites)
		{
			updateSatPlanet(sat, earth, t, dt0 * warp);
		}
	}
}

void updateTime()
{
	uint64_t raw = glfwGetTimerValue();
	t1 = Time(raw, raw / divisor);

	uint64_t delta = raw - t0.asU64();
	dt += Time(delta * warp);

	if (dt > dt0 * (maxUpdates * warp))
	{
		dt = dt0 * (maxUpdates * warp);
	}

	uint64_t time = t.asU64() + dt.asU64();
	t  = Time(time, time / divisor);
	dt = Time(dt.asU64(), dt.asU64() / divisor);

	t0 = t1;
}

void update()
{
	updateTime();

	updateObjects();
	updatePhysics();
}


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
		ImGui::Text("Time raw: %llu", t.asU64()  );
		ImGui::Text("Time    : %f"  , t.asFloat());
		ImGui::Text("Divisor : %f", divisor);

		ImGui::SliderScalar("Time warp: ", ImGuiDataType_S64, &warp, &warpMin, &warpMax);
		warp = std::min(warp, warpMax);
		warp = std::max(warp, warpMin);

		//integrator(combo)
	}
}

void planetOptions()
{
	if (ImGui::CollapsingHeader("Planet parameters"))
	{
		//rotation

		//mass

		//size

		//pos
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

	if (sat->mOrbit)
	{
		auto& orbit = *(sat->mOrbit);

		ImGui::Text("Main:");
		ImGui::Text("Specific ang. momentum : %f", orbit.mC);
		ImGui::Text("Inclination            : %f", orbit.mI);
		ImGui::Text("Right ascension        : %f", orbit.mRA);
		ImGui::Text("Eccentricity           : %f", orbit.mE);
		ImGui::Text("Argument of perigee    : %f", orbit.mAP);
		ImGui::Text("True anomaly           : %f", orbit.mTA);
		ImGui::Text("");	

		ImGui::Text("Spec: ");
		ImGui::Text("Orbit period      : %f", orbit.mOP);
		ImGui::Text("Apoapsis          : %f", orbit.mA);
		ImGui::Text("Eccentric anomaly : %f", orbit.mEA);
		ImGui::Text("Time              : %f", orbit.mT);
		ImGui::Text("");
	}
}

void satellitesOptions()
{
	if (ImGui::CollapsingHeader("Satellites parameters"))
	{
		//flags : single flag
		int flags = ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_HeightRegular;

		//choose satellite
		auto current = currentWeak.lock();
		const char* init = (current ? current->mName->mName.c_str() : "none");
		if (ImGui::BeginCombo("Satellites", init, flags))
		{
			bool selected = false;

			for (auto&[key, sat] : satellites)
			{
				auto& name = sat->mName->mName;

				selected = (current == sat);
				if (ImGui::Selectable(name.c_str(), selected))
				{
					currentWeak = sat;
					timeTrack0 = sat->mOrbit->mT;
					timeTrack1 = sat->mOrbit->mT;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Separator();

		//show params
		if (current)
		{
			showSatelliteParameters(current);

			//show tracked time
			ImGui::Separator();
			timeTrack0 = timeTrack1;
			timeTrack1 = current->mOrbit->mT;
			ImGui::Text("Tracked delta: %f", timeTrack1 - timeTrack0);
		}
		else
		{
			ImGui::Text("Nothing selected");
		}
	}
}

void myGui()
{
	ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints({200, 400}, {400, 800}, nullptr, nullptr);

	ImGui::Begin(
		"Options"
		, nullptr
		, ImGuiWindowFlags_MenuBar 
		| ImGuiWindowFlags_NoMove 
		| ImGuiWindowFlags_NoNav 
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_AlwaysAutoResize
	);
	
	systemOptions();

	planetOptions();

	satellitesOptions();

	ImGui::End();
}


//gui init/render/destroy
void initGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

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
	glfwMakeContextCurrent(window);

	for (auto&[name, renderer] : renderers)
	{
		renderer->render(view);
	}

	//test: TODO(add renderer + point builder)
	static Mat4    rot = glm::rotate(Mat4(1.0), glm::radians(+3.0), Vec3(1.0, 1.0, 1.0));
	static Mat4 invRot = glm::rotate(Mat4(1.0), glm::radians(-3.0), Vec3(1.0, 1.0, 1.0));

	glClear(GL_DEPTH_BUFFER_BIT);
	axes->use();
	axes->setUniformMat4(uProj, view.proj());
	axes->setUniformMat4(uView, view.view());
	for (auto&[key, sat] : satellites)
	{
		auto& mat      = sat->mPhysics->mMat;
		auto& pos = sat->mPhysics->mPosition;

		axes->setUniformMat4(uModel, mat);
		mat      = mat * rot;
		mat[3] = Vec4(pos, 1.0);

		glDrawArrays(GL_POINTS, 0, 1);
	}
}


void featureTest()
{
	initGlobals();
	initGui();


    //setups
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	
	glfwSetWindowPos(window, mode->width / 2 - WIDTH / 2, mode->height / 2 - HEIGHT / 2	);
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);
    glfwSetCursorPos(window, prevX, prevY);
	glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();

		if (!stopped)
		{
			update();
		}

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