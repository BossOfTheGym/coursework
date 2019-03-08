#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <filesystem>


#include <glew/glew.h>


#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const float PI   = static_cast<float>(acos(-1.0));
const float PI_2 = static_cast<float>(2 * PI);


using Vec4 = glm::vec4;
using Mat4 = glm::mat4;

using Vec3 = glm::vec3;
using Mat3 = glm::mat3;


using String = std::string;

using IStream = std::istream;
using OStream = std::ostream;

using IFStream = std::ifstream;
using OFStream = std::ofstream;