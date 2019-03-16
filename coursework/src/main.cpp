#include <Common.h>

#include <Camera/Camera.h>

#include <Shader/Shader.h>
#include <Shader/ShaderProgram.h>

#include <Texture/Texture2D.h>

#include <Model/VertexArrayBuffer.h>
#include <Model/Model.h>


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>


const int WIDTH  = 1200;
const int HEIGHT = 900;
 

using namespace std::chrono;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
//
void posCallback(GLFWwindow* window, double xPos, double yPos);
//
void errorCallback(int error, const char* description);

GLFWwindow* createWindow()
{
    auto glfwErr = glfwInit();

    GLFWwindow* win;

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    win = glfwCreateWindow(WIDTH, HEIGHT, "PINGAS PROD", nullptr, nullptr);

    glfwSetKeyCallback(win, keyCallback);
    glfwSetCursorPosCallback(win, posCallback);
    glfwSetErrorCallback(errorCallback);

    glfwMakeContextCurrent(win);

    auto glewErr = glewInit();

    return win;
}



void pushVertex(std::vector<float>& data, const glm::vec3& vertex, const glm::vec3& color, const glm::vec2& tex = glm::vec2())
{
    data.push_back(vertex.x);
    data.push_back(vertex.y);
    data.push_back(vertex.z);

    data.push_back(color.x);
    data.push_back(color.y);
    data.push_back(color.z);

    data.push_back(tex.s);
    data.push_back(tex.t);
}

std::vector<float> getIcosahedron(int split = 0)
{
    const int PENTAGON_SIDES = 5;

    const float HALF_ANGLE = PI / PENTAGON_SIDES;
    const float ANGLE      = 2 * HALF_ANGLE;
    const float SIN_HALF   = sin(HALF_ANGLE);

    const float ICOSAHEDRON_SIDE = sqrt(4 * pow(SIN_HALF, 2) - 1.0f) / SIN_HALF;
    const float PYRAMID_HEIGHT   = pow(ICOSAHEDRON_SIDE, 2) / 2;
    const float RADIUS           = ICOSAHEDRON_SIDE / (2 * SIN_HALF);
    const float HEIGHT_ANGLE = atan((1.0f - PYRAMID_HEIGHT) / RADIUS);

    const float A   = ICOSAHEDRON_SIDE;
    const float H   = PYRAMID_HEIGHT;
    const float R   = RADIUS;
    const float PHI = HEIGHT_ANGLE;

    const int N = PENTAGON_SIDES;


    //init icosahedron verteces
    glm::vec3 top    = glm::vec3(0.0f, 0.0f, +1.0f);
    glm::vec3 bottom = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3    topRing[N];
    glm::vec3 bottomRing[N];

    //tex coords
    glm::vec2 topTex    = glm::vec2(0.5f, 1.0f);
    glm::vec2 bottomTex = glm::vec2(0.5f, 0.0f);

    glm::vec2 topRingTex[N];
    glm::vec2 bottomRingTex[N];


    float angle;
    for (int i = 0; i < N; i++) 
    {
        angle = ANGLE * i;
        topRing[i]    = glm::vec3(R * cos(angle), R * sin(angle), 1.0f - H);
        topRingTex[i] = glm::vec2(angle / PI_2, 2 * PHI / PI + 0.5f);

        angle = ANGLE * i + HALF_ANGLE;
        bottomRing[i]    = glm::vec3(R * cos(angle), R * sin(angle), H - 1.0f);
        bottomRingTex[i] = glm::vec2(angle / PI_2, -2 * PHI / PI + 0.5f);
    }


    //assemble triangles
    std::vector<float> icosahedron;
    for (int curr = 0; curr < N; curr++)
    {
        int prev = (curr + N - 1) % N;
        int next = (curr + N + 1) % N;

        //top + topRing[i] + topRing[i + 1]
        pushVertex(icosahedron, top          , glm::vec3(1.0f, 0.0f, 0.0f), topTex);
        pushVertex(icosahedron, topRing[curr], glm::vec3(0.0f, 1.0f, 0.0f), topRingTex[curr]);
        pushVertex(icosahedron, topRing[next], glm::vec3(0.0f, 1.0f, 0.0f), topRingTex[next]);

        //bottom + bottomRing[i] + bottomRing[i + 1]
        pushVertex(icosahedron, bottom          , glm::vec3(0.0f, 0.0f, 1.0f), bottomTex);
        pushVertex(icosahedron, bottomRing[next], glm::vec3(0.0f, 1.0f, 0.0f), bottomRingTex[next]);
        pushVertex(icosahedron, bottomRing[curr], glm::vec3(0.0f, 1.0f, 0.0f), bottomRingTex[curr]);

        //bottomRing[i] + topRing[i] + topRing[i + 1]
        pushVertex(icosahedron, bottomRing[curr], glm::vec3(1.0f, 0.0f, 0.0f), bottomRingTex[curr]);
        pushVertex(icosahedron, topRing[next]   , glm::vec3(0.0f, 1.0f, 0.0f), topRingTex[next]);
        pushVertex(icosahedron, topRing[curr]   , glm::vec3(0.0f, 1.0f, 0.0f), topRingTex[curr]);

        //topRing[i] + bottomRing[i] + bottomRing[i - 1]
        pushVertex(icosahedron, topRing[curr]   , glm::vec3(0.0f, 0.0f, 1.0f), topRingTex[curr]);
        pushVertex(icosahedron, bottomRing[prev], glm::vec3(0.0f, 1.0f, 0.0f), bottomRingTex[prev]);
        pushVertex(icosahedron, bottomRing[curr], glm::vec3(0.0f, 1.0f, 0.0f), bottomRingTex[curr]);
    }

    return icosahedron;
}

VertexArrayBuffer createIcosahedron()
{
    VertexArrayBuffer ico(60, getIcosahedron());

    ico.bindArray();
    ico.setAttribPointer(0, 8 * sizeof(float), (void *)(0));
    ico.setAttribPointer(1, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    ico.setAttribPointer(2, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    
    return ico;
}



std::map<String, Shader> loadShaders()
{    
    std::map<String, Shader> shaders;

    shaders.insert({"assets/shaders/sphere.vs", Shader(Shader::Vertex, "assets/shaders/sphere.vs")});
    shaders.insert({"assets/shaders/sphere.tcs", Shader(Shader::TessControl, "assets/shaders/sphere.tcs")});
    shaders.insert({"assets/shaders/sphere.tes", Shader(Shader::TessEvaluation, "assets/shaders/sphere.tes")});
    shaders.insert({"assets/shaders/sphere.gs", Shader(Shader::Geometry, "assets/shaders/sphere.gs")});
    shaders.insert({"assets/shaders/sphere.fs", Shader(Shader::Fragment, "assets/shaders/sphere.fs")});

    shaders.insert({"assets/shaders/setellite.fs", Shader(Shader::Fragment, "assets/shaders/satellite.fs")});


    for (const auto&[location, shader] : shaders)
    {
        if (!shader.compiled())
        {
            std::cerr << "FAILED TO COMPILE SHADER: " << location << std::endl;
            std::cerr << shader.getInfoLog();
        }
    }

    return shaders;
}



Texture2D loadEarth()
{
    Texture2D earthTex("assets/textures/earth/earthmap1k.jpg");
    earthTex.bind();
    earthTex.texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP);
    earthTex.texParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP);
    earthTex.texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    earthTex.texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return earthTex;
}



ShaderProgram createPlanetProgram(std::map<String, Shader>& shadersHolder)
{
    ShaderProgram program;
    program.attachShader(shadersHolder["assets/shaders/sphere.vs"]);
    program.attachShader(shadersHolder["assets/shaders/sphere.tcs"]);
    program.attachShader(shadersHolder["assets/shaders/sphere.tes"]);
    program.attachShader(shadersHolder["assets/shaders/sphere.gs"]);
    program.attachShader(shadersHolder["assets/shaders/sphere.fs"]);

    program.link();
    if (!program.linked())
    {
        std::cerr << program.getInfoLog();
    }

    return program;
}

ShaderProgram createSatelliteProgram(std::map<String, Shader>& shadersHolder)
{
    ShaderProgram program;
    program.attachShader(shadersHolder["assets/shaders/sphere.vs"]);
    program.attachShader(shadersHolder["assets/shaders/sphere.tcs"]);
    program.attachShader(shadersHolder["assets/shaders/sphere.tes"]);
    program.attachShader(shadersHolder["assets/shaders/satellite.fs"]);

    program.link();
    if (!program.linked())
    {
        std::cerr << program.getInfoLog();
    }

    return program;
}



//globals
GLFWwindow* window = createWindow();

std::map<String, Shader> shaders = loadShaders();

Texture2D earth = loadEarth();

ShaderProgram planetProgram = createPlanetProgram(shaders);
ShaderProgram satelliteProgram = createSatelliteProgram(shaders);

GLint model = planetProgram.getUniformLocation("model");
GLint view = planetProgram.getUniformLocation("view");
GLint projection = planetProgram.getUniformLocation("projection");
GLint lightPos = planetProgram.getUniformLocation("lightPos");
GLint lightColor = planetProgram.getUniformLocation("lightColor");
GLint inner = planetProgram.getUniformLocation("inner");
GLint outer = planetProgram.getUniformLocation("outer");

VertexArrayBuffer icosahedron = createIcosahedron();

Camera camera(glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), Vec3(0.0f, 0.0f, 30.0f));

bool fill = false;

float innerTess = 10.0f;
float outerTess = 10.0f;
float deltaTess = 0.2f;

double prevX = WIDTH / 2;
double prevY = HEIGHT / 2;




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
            camera.travelView(0.5f);

            break;
        }
        case (GLFW_KEY_S):
        {
            camera.travelView(-0.5f);

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
    if (prevX != -1 && prevY != -1)
    {
        camera.rotate(float(xPos - prevX), float(yPos - prevY));
    }
    prevX = xPos;
    prevY = yPos;
}

void errorCallback(int error, const char* msg)
{
    std::cout << "Error callback" << std::endl;

    std::cout << "ERROR: " << error << std::endl;
    std::cout << "MSG: " << msg << std::endl;
}


void featureTest()
{
    //setups
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);


    //loop params
    glm::mat4 matProj = glm::perspective(glm::radians(45.0f), 1.0f * WIDTH / HEIGHT, 0.1f, 150.0f);
    glm::mat4 matModel = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f));

    glm::mat4 second = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(1.0f, 0.0f, 1.0f));

    glm::vec3 vecLightPos   = glm::vec3(50.0f, 50.0f, 0.0f);
    glm::vec3 vecLightColor = glm::vec3(1.0f);


    //orbit
    glm::vec3 r(0.0f, 7.0f, 0.0f);
    glm::vec3 v(16.0f, 0.0f, 0.0f);

    const float GM = 2000.0;

    double t0;
    double t1;
    double delta;

    t0 = glfwGetTime();
    glfwSetCursorPos(window, prevX, prevY);
    while (!glfwWindowShouldClose(window))
    {
        //prepare
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        planetProgram.use();
        planetProgram.setUniform1f(inner, innerTess);
        planetProgram.setUniform1f(outer, outerTess);
        planetProgram.setUniformVec3(lightPos, vecLightPos);
        planetProgram.setUniformVec3(lightColor, vecLightColor);
        planetProgram.setUniformMat4(projection, matProj);
        planetProgram.setUniformMat4(view, camera.mat());


        //render
        Texture2D::active(GL_TEXTURE0);
        earth.bind();
        icosahedron.bindArray();


        //planet
        planetProgram.setUniformMat4(model, matModel);
        glDrawArrays(GL_PATCHES, 0, 60);
        

        //sat
        auto m = second;
        m[3] = Vec4(r, 1.0f);

        planetProgram.setUniformMat4(model, m);
        glDrawArrays(GL_PATCHES, 0, 60);


        //update
        matModel = rotation * matModel;
        second   = rotation * second;


        //delta
        t1 = glfwGetTime();
        delta = (t1 - t0);
        t0 = t1;

        auto vj = v;
        auto rj = r;
        auto dot = glm::dot(r, r);
        auto ur = glm::normalize(r);

        float dt = delta / 10;

        v -= dt * GM / dot * ur;
        r += dt * vj;

        std::this_thread::sleep_for(10ms);

        //swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}




std::ostream& offset(int shift)
{
    for (int i = 0; i < shift; i++)
    {
        std::cout << "    ";
    }

    return std::cout;
}

void processMesh(int shift, const aiMesh* mesh)
{
    offset(shift) << "Name: " << mesh->mName.C_Str() << std::endl;
    offset(shift) << "Color channels: " << mesh->GetNumColorChannels() << std::endl;
    offset(shift) << "UV channels: " << mesh->GetNumUVChannels() << std::endl;
    offset(shift) << "Has bones: " << mesh->HasBones() << std::endl;
    offset(shift) << "Has faces: " << mesh->HasFaces() << std::endl;
    offset(shift) << "Has normals: " << mesh->HasNormals() << std::endl;
    offset(shift) << "Has positions: " << mesh->HasPositions() << std::endl;
    offset(shift) << "Has tangents and bitangents: " << mesh->HasTangentsAndBitangents() << std::endl;
}

void processNode(int shift, const aiNode* node)
{
    offset(shift) << "Node: " << node->mName.C_Str() << std::endl;


    offset(shift) << "Transformation: " << std::endl;
    auto& transform = node->mTransformation;
    offset(shift) << transform[0][0] << transform[0][1] << transform[0][2] << transform[0][3] << std::endl;
    offset(shift) << transform[1][0] << transform[1][1] << transform[1][2] << transform[1][3] << std::endl;
    offset(shift) << transform[2][0] << transform[2][1] << transform[2][2] << transform[2][3] << std::endl;
    offset(shift) << transform[3][0] << transform[3][1] << transform[3][2] << transform[3][3] << std::endl;

    
    offset(shift) << "Meshes: " << node->mNumMeshes << std::endl;
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        offset(shift) << "Mesh: " << node->mMeshes[i] << std::endl;
    }


    offset(shift) << "Children: " << node->mNumChildren << std::endl;
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(shift + 1, node->mChildren[i]);
    }
}

void processScene(const aiScene* scene)
{
    std::cout << "Has animations: " << scene->HasAnimations() << std::endl;
    std::cout << "Has cameras: " << scene->HasCameras() << std::endl;
    std::cout << "Has lights: " << scene->HasLights() << std::endl;
    std::cout << "Has materials: " << scene->HasMaterials() << std::endl;
    std::cout << "Has meshes: " << scene->HasMeshes() << std::endl;
    std::cout << "Has textures: " << scene->HasTextures() << std::endl;

    std::cout << "Animations: " << scene->mNumAnimations << std::endl;
    std::cout << "Cameras: " << scene->mNumCameras << std::endl;
    std::cout << "Lights: " << scene->mNumLights << std::endl;
    std::cout << "Materials: " << scene->mNumMaterials << std::endl;
    std::cout << "Meshes: " << scene->mNumMeshes << std::endl;
    std::cout << "Textures: " << scene->mNumTextures << std::endl;


    std::cout << "Meshes: " << std::endl;
    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        processMesh(0, scene->mMeshes[i]);
    }


    processNode(0, scene->mRootNode);
}



bool importModel()
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        "assets/textures/Satellite/10477_Satellite_v1_L3.obj",
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );
    
    if (!scene)
    {
        return false;
    }

    processScene(scene);

    return true;
}

void testAssimp()
{
    importModel();
}




int main(int argc, char* argv[])
{
    featureTest();

    //testAssimp();



    return EXIT_SUCCESS;
}