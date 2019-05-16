#include <Objects/View.h>

#include <Resources.h>
#include <Factories.h>

#include <Numerics/Ivp/RungeKutta.h>

#include <Rendezvous/Circular/Circular.h>
#include <Rendezvous/RendezvousControl.h>

#include <GUI/GUI.h>

#include <Clock/Clock.h>


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

//rendezvous control
RendezvousControlShared rendezvousControl;

//gui state
double transferVal;
IntegratorShared selectedInt;

//flags
bool fill;          // wireframe mode
bool stopped;       // enable / disable objects update
bool scrollView;    // scroll view

//screen pos
double prevX;
double prevY;

//speed
double speed;
double speedMin;
double speedMax;

//tick
Clock tick;

//updates
uint64_t updates;
uint64_t updatesMin;
uint64_t updatesMax;


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
            view.travelView(speed, View::Z);

            break;
        }
        case (GLFW_KEY_S):
        {
            view.travelView(-speed, View::Z);

            break;
        }
		case (GLFW_KEY_A):
		{
			view.travelView(speed, View::X);

			break;
		}
		case (GLFW_KEY_D):
		{
			view.travelView(-speed, View::X);

			break;
		}
		case (GLFW_KEY_Q):
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);

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
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) || ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow))
	{
		scrollView = false;

		return;
	}

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

	Context::init(WIDTH, HEIGHT, "Rendezvous");
	window = Context::getContext()->window();
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, posCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	glfwSetWindowPos(window, mode->width / 2 - WIDTH / 2, mode->height / 2 - HEIGHT / 2);
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);
	glfwSetCursorPos(window, prevX, prevY);
	glfwSwapInterval(1);

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
		, glm::perspective(glm::radians(60.0), 1.0 * WIDTH / HEIGHT, 0.001, 10 * R)
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
		, Vec3(1.0, 1.0, 1.0)
		, glm::scale(Mat4(1.0), Vec3(1.0))
		, Vec3(R + 420.0, 0.0, 0.0)
		, Vec3(0.0, 0.0, sqrt(MU / (R + 400.0)))
		, "target"
		, earth->mPhysics
	);

	//chaser
	chaser = createSatellite(
		models["satellite"]
		, 1.0
		, Vec3(0.0, 1.0, 0.0)
		, glm::scale(Mat4(1.0), Vec3(1.0))
		, Vec3(R + 420.0, 0.0, 0.0)
		, Vec3(0.0, 0.0, sqrt(MU / (R + 400.0)))
		, "chaser"
		, earth->mPhysics
	);

	//rendezvous
	rendezvousControl = RendezvousControlShared(
		new RendezvousControl(
			MethodUnique(new Circular())
		)
	);

	//render lists
	renderers["satellite"]->addToList(target);
	renderers["satellite"]->addToList(chaser);
	renderers["planet"]->addToList(earth);

	//gui
	selectedInt = integrators["classic4"];
	transferVal = 3600.0;

	//loop states
	fill    = false;
	stopped = true;
	scrollView = false;

	prevX = WIDTH / 2;
	prevY = HEIGHT / 2;

	speed = 0.001;
	speedMin = 0.001;
	speedMax = 1000.0;

	//time step
	tick = Clock(100, 1, 1, 2000, (double)glfwGetTimerFrequency());

	updates = 1;
	updatesMin = 1;
	updatesMax = 7500;
}

void destroyGlobals()
{
	Context::terminate();
}


//main loop

//integrator(3 lines)
void updateSatPlanet(SatelliteShared& sat, PlanetShared& planet, const Time& t, const Time& dt)
{	
	selectedInt->updatePhysics(planet, sat, t, dt);
}

void updateObjects()
{
	rendezvousControl->update(tick.elapsedWarped(), tick.deltaWarped());

	earth->update(tick.elapsedWarped(), tick.deltaWarped());

	target->update(tick.elapsedWarped(), tick.deltaWarped());
	chaser->update(tick.elapsedWarped(), tick.deltaWarped());
}

void updatePhysics()
{
	updateSatPlanet(target, earth, tick.elapsedWarped(), tick.deltaWarped());
	updateSatPlanet(chaser, earth, tick.elapsedWarped(), tick.deltaWarped());
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

		ImGui::Text("Time raw : %llu", tick.elapsedWarped().asU64());
		ImGui::Text("Time     : %f"  , tick.elapsedWarped().asFloat());
		ImGui::Text("Delta raw: %llu", tick.deltaWarped().asU64());
		ImGui::Text("Delta    : %f"  , tick.deltaWarped().asFloat());
		ImGui::Text("Divisor: %f", tick.getDivisor());

		uint64_t warp = tick.getWarp();
		uint64_t warpMin = tick.getMinWarp();
		uint64_t warpMax = tick.getMaxWarp();
		ImGui::SliderScalar("Time warp", ImGuiDataType_S64, &warp, &warpMin, &warpMax);
		tick.setWarp(glm::clamp(warp, warpMin, warpMax));

		ImGui::SliderScalar("Updates", ImGuiDataType_S64, &updates, &updatesMin, &updatesMax);
		updates = glm::clamp(updates, updatesMin, updatesMax);


		if (ImGui::BeginCombo("Integrators", selectedInt->name().c_str(), ImGuiComboFlags_NoArrowButton))
		{
			for (auto&[key, integrator] : integrators)
			{
				bool selected = (integrator == selectedInt);

				auto& name = integrator->name();
				if (ImGui::Selectable(name.c_str(), &selected))
				{
					selectedInt = integrator;
				}
			}
			ImGui::EndCombo();
		}


		static const char* options[] = {"pause simulation", "resume simulation"};
		if (ImGui::Button(options[stopped]))
		{
			stopped ^= true;
		}

		ImGui::Text("");
		ImGui::SliderScalar("Speed", ImGuiDataType_Double, &speed, &speedMin, &speedMax, "%.6f", 1.0f);
		ImGui::Text("View position     : x:%f y:%f z:%f", view.pos().x, view.pos().y, view.pos().z);
		ImGui::Text("View last position: x:%f y:%f z:%f", view.lastPos().x, view.lastPos().y, view.lastPos().z);
	}
}

void planetOptions()
{
	if (ImGui::CollapsingHeader("Planet parameters"))
	{
		ImGui::Text("Not implemented yet");
	}
}

void showParameters(SatelliteShared& sat, int id)
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

void adjustParameters(SatelliteShared& sat, int id)
{
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

		
		char am[64];
		char  e[64];
		double amVal = orbit.angularMomentum();
		double  eVal = orbit.eccentricity();

		sprintf_s(am, "%.15f", amVal);
		sprintf_s( e, "%.15f", eVal);

		ImGui::InputText("angular momentum(h)", am, 64, ImGuiInputTextFlags_CharsDecimal);
		ImGui::InputText("eccentricity(e)" ,     e, 64, ImGuiInputTextFlags_CharsDecimal);

		amVal = atof(am);
		eVal  = atof(e);


		using glm::radians;
		using glm::degrees;

		double DEGREE_0 = 0.0, DEGREE_360 = 360.0, DEGREE_180 = 180.0;

		double ra = degrees(orbit.rightAscension());
		double ap = degrees(orbit.argumentOfPeriapsis());
		double i  = degrees(orbit.inclination());
		double ta = degrees(orbit.trueAnomaly());

		ImGui::SliderScalar("right ascension(Omega)"    , ImGuiDataType_Double, &ra, &DEGREE_0, &DEGREE_360, "%.15f", 1.0f);
		ImGui::SliderScalar("argument of perigee(omega)", ImGuiDataType_Double, &ap, &DEGREE_0, &DEGREE_360, "%.15f", 1.0f);
		ImGui::SliderScalar("inclination(i)"            , ImGuiDataType_Double,  &i, &DEGREE_0, &DEGREE_180, "%.15f", 1.0f);
		ImGui::SliderScalar("true anomaly(theta)"       , ImGuiDataType_Double, &ta, &DEGREE_0, &DEGREE_360, "%.15f", 1.0f);
		

		orbit.setOrbit(amVal, eVal, radians(i), radians(ra), radians(ap), radians(ta));


		ImGui::Text("");
	}
}

void showSatelliteParameters(SatelliteShared& sat, int id)
{
	ImGui::PushID(id);

	if (!stopped)
	{
		showParameters(sat, id);
	}
	else
	{
		adjustParameters(sat, id);
	}

	if (ImGui::Button("Set view"))
	{
		view.setTrack(sat->mPhysics, Vec3(0.030, 0.0, 0.0));
	}

	ImGui::PopID();
}

void satellitesOptions()
{
	if (ImGui::CollapsingHeader("Target"))
	{
		showSatelliteParameters(target, 12345678);
	}
	if (ImGui::CollapsingHeader("Chaser"))
	{
		showSatelliteParameters(chaser, 11111111);
	}
}

void rendezvousOptions()
{
	if (ImGui::CollapsingHeader("Rendezvous"))
	{
		auto chaserObj = rendezvousControl->getChaser().lock();
		auto targetObj = rendezvousControl->getTarget().lock();
		if (chaserObj && targetObj)
		{
			Vec3 dr = targetObj->mPhysics->mPosition - chaserObj->mPhysics->mPosition;
			Vec3 dv = targetObj->mPhysics->mVelocity - chaserObj->mPhysics->mVelocity;

			ImGui::Text("delta-r:%f  x:%f y:%f z:%f", glm::length(dr), dr.x, dr.y, dr.z);
			ImGui::Text("delta-v:%f  x:%f y:%f z:%f", glm::length(dv), dv.x, dv.y, dv.z);
		}

		if (rendezvousControl->finished())
		{
			char transfer[64];

			sprintf_s(transfer, "%.15f", transferVal);

			ImGui::InputText("time", transfer, 64, ImGuiInputTextFlags_CharsDecimal);

			transferVal = atof(transfer);


			if (ImGui::Button("Start"))
			{
				rendezvousControl->setChaser(chaser);
				rendezvousControl->setTarget(target);
				rendezvousControl->setTime(tick.fromFloat(transferVal));
				rendezvousControl->start();
			}
		}
		else
		{
			ImGui::Text("Time remaining: %f", rendezvousControl->getTime().asFloat());

			if (ImGui::Button("Adjust"))
			{
				rendezvousControl->start();
			}
			if (ImGui::Button("Stop"))
			{
				rendezvousControl->stop();
			}
		}
	}
}

void myGui()
{
	ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints({200, 400}, {500, 800}, nullptr, nullptr);

	ImGui::Begin("Options", nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
	
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

void beginGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void constructGui()
{
	//testGui();

	myGui();
}

void endGui()
{
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


void mainloop()
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
	
    while (!glfwWindowShouldClose(window))
    {
		glfwPollEvents();


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		beginGui();

		//update section
		tick.updateDelta();
		tick.updateDeltaWarped();
		if (!stopped)
		{
			for (int i = 0; i < updates; i++)
			{
				updateObjects();
				updatePhysics();

				tick.updateTimeWarped();
			}
		}
		view.update(tick.elapsed(), tick.delta());

		tick.updateTime();
		//end update section

		constructGui();


		render();

		endGui();


		glfwSwapBuffers(window);
    }

	destroyGui();
	destroyGlobals();
}

 

int main(int argc, char* argv[])
{
    mainloop();
	
    return EXIT_SUCCESS;
}