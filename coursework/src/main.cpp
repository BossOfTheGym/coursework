#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <filesystem>


#include <FreeImage/FreeImage.h>


#include <glew/glew.h>


#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Camera/Camera.h"


using namespace std::chrono_literals;
namespace fs = std::filesystem;

const int WIDTH  = 1200;
const int HEIGHT = 900;

const float PI   = acos(-1.0);
const float PI_2 = 2 * PI;

//xyz -> str
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

void genBuffers(GLuint& vao, GLuint& vbo, std::vector<float>& data)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);


    glBindVertexArray(vao);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
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

    //earth
    GLuint earthTexture;
    glGenTextures(1, &earthTexture);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    char earthLocation[] = "assets/textures/earth/earthmap1k.jpg";
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(earthLocation, 0);
    if (format == -1)
    {
        std::cout << "Could not find image" << " - Aborting." << std::endl;
        return;
    }
    if (format == FIF_UNKNOWN)
    {
        std::cout << "Couldn't determine file format - attempting to get from file extension..." << std::endl;

        format = FreeImage_GetFIFFromFilename(earthLocation);

        if (!FreeImage_FIFSupportsReading(format))
        {
            std::cout << "Detected image format cannot be read!" << std::endl;
            return;
        }
    }
    
    FIBITMAP* bitmap = FreeImage_Load(format, earthLocation);
    int bitsPerPixel = FreeImage_GetBPP(bitmap);

    FIBITMAP* bitmap32;
    if (bitsPerPixel == 32)
    {
        std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << std::endl;
        bitmap32 = bitmap;
    }
    else
    {
        std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << std::endl;
        bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    }

    int imageWidth = FreeImage_GetWidth(bitmap32);
    int imageHeight = FreeImage_GetHeight(bitmap32);

    GLubyte* textureData = FreeImage_GetBits(bitmap32);
    glTexImage2D(GL_TEXTURE_2D,    // Type of texture
        0,                // Mipmap level (0 being the top level i.e. full size)
        GL_RGBA,          // Internal format
        imageWidth,       // Width of the texture
        imageHeight,      // Height of the texture,
        0,                // Border in pixels
        GL_BGRA,          // Data format
        GL_UNSIGNED_BYTE, // Type of texture data
        textureData);     // The image data to use for this texture


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


    //setups
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glUseProgram(shaderProgram);

    window.setActive(true);


    //loop params
    glm::mat4 matProj  = glm::perspective(glm::radians(45.0f), 1.0f * WIDTH / HEIGHT, 1.0f, 150.0f);
    glm::mat4 matView  = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 matModel = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));

    Camera camera(matView, Vec3(0.0f, 0.0f, 30.0f));

    glm::mat4 second   = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(1.0f, 0.0f, 1.0f));

    glm::vec3 vecLightPos   = glm::vec3(100.0f, 100.0f, 0.0f);
    glm::vec3 vecLightColor = glm::vec3(1.0f);

    //orbit
    glm::vec3 r(0.0f, 7.0f, 0.0f);
    glm::vec3 v(23.0f, 0.0f, 0.0f);

    const float GM = 2000.0;


    sf::Clock tick;
    sf::Int64 time0 = tick.getElapsedTime().asMicroseconds();
    sf::Int64 time1;
    sf::Int64 delta;

    const sf::Int64 SLEEP = 10000;

    bool running = true;
    bool fill = false;

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
                    }

                    break;
                }
            }
        }

        //prepare
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glUniform3fv(lightPos, 1, &vecLightPos[0]);
        glUniform3fv(lightColor, 1, &vecLightColor[0]);

        glUniformMatrix4fv(projection, 1, GL_FALSE, &matProj[0][0]);
        glUniformMatrix4fv(view, 1, GL_FALSE, &camera.mat()[0][0]);


        //render
        //planet
        glUniformMatrix4fv(model, 1, GL_FALSE, &matModel[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);

        glBindVertexArray(vao);

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glDrawArrays(GL_PATCHES, 0, 60);

        //sat
        auto tr = glm::translate(glm::mat4(1.0f), r);
        auto m = tr * second;
        
        glUniformMatrix4fv(model, 1, GL_FALSE, &m[0][0]);

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glDrawArrays(GL_PATCHES, 0, 60);


        //update
        matModel = rotation * matModel;
        second = rotation * second;


        //delta
        time1 = tick.getElapsedTime().asMicroseconds();
        delta = time1 - time0;
        time0 = time1;

        auto vj = v;
        auto rj = r;
        auto dt = delta / (1e7f);
        auto dot = glm::dot(r, r);
        auto ur = glm::normalize(r);

        v -= dt * GM / dot * ur;
        r += dt * vj;


        //sleep
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