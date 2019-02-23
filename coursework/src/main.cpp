#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <thread>


#include <glew/glew.h>


#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std::chrono_literals;


const int WIDTH  = 800;
const int HEIGHT = 600;

const double PI = acos(-1.0);


void pushVertex(std::vector<float>& data, const glm::vec3& vertex, const glm::vec3& color, const glm::vec2& tex = glm::vec2())
{
    data.push_back(vertex.x);
    data.push_back(vertex.y);
    data.push_back(vertex.z);

    data.push_back(color.x);
    data.push_back(color.y);
    data.push_back(color.z);
}

void genBuffers(GLuint& vao, GLuint& vbo, std::vector<float>& data)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);


    glBindVertexArray(vao);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


std::vector<float> getPolygon(int verteces)
{
    const float delta = 2 * PI / verteces;

    std::vector<float> polygon;

    float angle;
    for (int i = 0; i < verteces; i++)
    {
        //center
        pushVertex(polygon, glm::vec3(0.0f), glm::vec3(1.0f));

        //i vertex
        angle = delta * i;
        pushVertex(polygon, glm::vec3(cos(angle), sin(angle), 0.0f), glm::vec3(1.0f));

        //i+1 vertex
        angle = delta * (i + 1);
        pushVertex(polygon, glm::vec3(cos(angle), sin(angle), 0.0f), glm::vec3(1.0f));
    }

    return polygon;
}

void createPolygon(GLuint& vao, GLuint& vbo, std::vector<float>& polygon, int vertices)
{
    polygon = getPolygon(vertices);

    genBuffers(vao, vbo, polygon);
}


std::vector<float> getIcosahedron()
{
    const int PENTAGON_SIDES = 5;

    const float HALF_ANGLE = PI / PENTAGON_SIDES;
    const float ANGLE      = 2 * HALF_ANGLE;
    const float SIN_HALF   = sin(HALF_ANGLE);

    const float ICOSAHEDRON_SIDE = sqrt(4 * pow(SIN_HALF, 2) - 1.0f) / SIN_HALF;
    const float PYRAMID_HEIGHT   = pow(ICOSAHEDRON_SIDE, 2) / 2;
    const float RADIUS           = ICOSAHEDRON_SIDE / (2 * SIN_HALF);

    const float A = ICOSAHEDRON_SIDE;
    const float H = PYRAMID_HEIGHT;

    const int N = PENTAGON_SIDES;


    //init icosahedron verteces
    glm::vec3 top    = glm::vec3(0.0f, 0.0f, +1.0f);
    glm::vec3 bottom = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3    topRing[N];
    glm::vec3 bottomRing[N];

    float angle;
    for (int i = 0; i < N; i++) 
    {
        angle = ANGLE * i;
        topRing[i]    = glm::vec3(RADIUS * cos(angle), RADIUS * sin(angle), 1.0f - H);

        angle = ANGLE * i + HALF_ANGLE;
        bottomRing[i] = glm::vec3(RADIUS * cos(angle), RADIUS * sin(angle), H - 1.0f);
    }


    //assemble triangles
    std::vector<float> icosahedron;
    for (int curr = 0; curr < N; curr++)
    {
        int prev = (curr + N - 1) % N;
        int next = (curr + N + 1) % N;

        //top + topRing[i] + topRing[i + 1]
        pushVertex(icosahedron, top          , glm::vec3(1.0f, 0.0f, 0.0f));
        pushVertex(icosahedron, topRing[curr], glm::vec3(0.0f, 1.0f, 0.0f));
        pushVertex(icosahedron, topRing[next], glm::vec3(0.0f, 1.0f, 0.0f));

        //bottom + bottomRing[i] + bottomRing[i + 1]
        pushVertex(icosahedron, bottom          , glm::vec3(0.0f, 0.0f, 1.0f));
        pushVertex(icosahedron, bottomRing[next], glm::vec3(0.0f, 1.0f, 0.0f));
        pushVertex(icosahedron, bottomRing[curr], glm::vec3(0.0f, 1.0f, 0.0f));

        //bottomRing[i] + topRing[i] + topRing[i + 1]
        pushVertex(icosahedron, bottomRing[curr], glm::vec3(1.0f, 0.0f, 0.0f));
        pushVertex(icosahedron, topRing[next]   , glm::vec3(0.0f, 1.0f, 0.0f));
        pushVertex(icosahedron, topRing[curr]   , glm::vec3(0.0f, 1.0f, 0.0f));

        //topRing[i] + bottomRing[i] + bottomRing[i - 1]
        pushVertex(icosahedron, topRing[curr]   , glm::vec3(0.0f, 0.0f, 1.0f));
        pushVertex(icosahedron, bottomRing[prev], glm::vec3(0.0f, 1.0f, 0.0f));
        pushVertex(icosahedron, bottomRing[curr], glm::vec3(0.0f, 1.0f, 0.0f));
    }

    return icosahedron;
}

void createIcosahedron(GLuint& vao, GLuint& vbo, std::vector<float>& icosahedron)
{
    icosahedron = getIcosahedron();

    genBuffers(vao, vbo, icosahedron);
}


std::string getShaderString(const std::string& fileDist)
{
    std::ifstream file(fileDist);

    std::string shader;
    std::string line;

    while (file.good())
    {
        std::getline(file, line);

        shader += line;
        shader += '\n';
    }

    return shader;
}

GLint createShader(GLuint& shader, GLenum type, const std::string& dest)
{
    const int BUFFER_SIZE = 1 << 12;

    std::string shaderString = getShaderString(dest);
    const char* source       = shaderString.c_str();

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // check for shader compile errors
    GLint success;
    static GLchar* infoLog = new GLchar[BUFFER_SIZE];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, BUFFER_SIZE, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return 0;
}


void testTesselation()
{
    //window
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 4;
    settings.minorVersion = 4;
    settings.stencilBits = 8;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "OpenGL", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    auto err = glewInit();


    //shaders
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint tessControlShader;
    GLuint tessEvaluationShader;

    createShader(vertexShader, GL_VERTEX_SHADER, "assets/shaders/sphere.vs");
    createShader(tessControlShader, GL_TESS_CONTROL_SHADER, "assets/shaders/sphere.tcs");
    createShader(tessEvaluationShader, GL_TESS_EVALUATION_SHADER, "assets/shaders/sphere.tes");
    createShader(fragmentShader, GL_FRAGMENT_SHADER, "assets/shaders/sphere.fs");


    //program
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, tessControlShader);
    glAttachShader(shaderProgram, tessEvaluationShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);


    //data
    std::vector<float> icosahedron;
    GLuint vao;
    GLuint vbo;
    createIcosahedron(vao, vbo, icosahedron);


    //uniforms
    GLint model = glGetUniformLocation(shaderProgram, "model");
    GLint view  = glGetUniformLocation(shaderProgram, "view");
    GLint projection  = glGetUniformLocation(shaderProgram, "projection");

    GLint lightPos = glGetUniformLocation(shaderProgram, "lightPos");
    GLint lightColor = glGetUniformLocation(shaderProgram, "lightColor");
    GLint ambient = glGetUniformLocation(shaderProgram, "ambient");
    GLint diffuse = glGetUniformLocation(shaderProgram, "diffuse");


    //setups
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(0.3f, 0.3f, 0.3, 1.0f);

    glUseProgram(shaderProgram);

    window.setActive(true);


    //loop params
    glm::mat4 matProj  = glm::perspective(glm::radians(45.0f), 1.0f * WIDTH / HEIGHT, 1.0f, 15.0f);
    glm::mat4 matView  = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 matModel = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(1.0f, 1.0f, 1.0f));

    glm::vec3 vecLightPos   = glm::vec3(12.0f, 8.0f, 0.0f);
    glm::vec3 vecLightColor = glm::vec3(1.0f);


    sf::Clock tick;
    sf::Int64 time0 = tick.getElapsedTime().asMicroseconds();
    sf::Int64 time1;
    sf::Int64 delta;

    const sf::Int64 SLEEP = 10000;

    bool running = true;
    bool fill = false;
    GLint ambientOn = true;
    GLint diffuseOn = true;

    glUniform1i(ambient, ambientOn);
    glUniform1i(diffuse, diffuseOn);
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    running = false;

                    break;
                }
                case sf::Event::KeyPressed:
                {
                    switch (event.key.code)
                    {
                        case (sf::Keyboard::C):
                        {
                            fill ^= true;
                            glPolygonMode(GL_FRONT_AND_BACK, (fill ? GL_FILL : GL_LINE));

                            break;
                        }

                        case (sf::Keyboard::A):
                        {
                            ambientOn ^= true;
                            glUniform1i(ambient, ambientOn);

                            break;
                        }

                        case (sf::Keyboard::D):
                        {
                            diffuseOn ^= true;
                            glUniform1i(diffuse, diffuseOn);
                            break;
                        }
                    }

                    break;
                }
            }
        }

        //prepare
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        matModel = rotation * matModel;

        glUniform3fv(lightPos, 1, &vecLightPos[0]);
        glUniform3fv(lightColor, 1, &vecLightColor[0]);

        glUniformMatrix4fv(projection, 1, GL_FALSE, &matProj[0][0]);
        glUniformMatrix4fv(view, 1, GL_FALSE, &matView[0][0]);
        glUniformMatrix4fv(model, 1, GL_FALSE, &matModel[0][0]);


        //render
        glBindVertexArray(vao);

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glDrawArrays(GL_PATCHES, 0, 60);

        //sleep
        time1 = tick.getElapsedTime().asMicroseconds();
        delta = time1 - time0;
        time0 = time1;

        if (delta < SLEEP)
        { 
            std::this_thread::sleep_for(std::chrono::microseconds(SLEEP - delta));
        }


        //display frame
        window.display();
    }

    //free
    glDeleteShader(vertexShader);
    glDeleteShader(tessControlShader);
    glDeleteShader(tessEvaluationShader);
    glDeleteShader(fragmentShader);

    glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}


int main(int argc, char* argv[])
{
    testTesselation();

    return EXIT_SUCCESS;
}