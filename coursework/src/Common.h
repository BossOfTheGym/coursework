#pragma once

#include <iostream>
#include <fstream>

#include <cmath>

#include <thread>
#include <memory>

#include <filesystem>
#include <functional>

#include <string>
#include <vector>
#include <map>


#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const float PI   = static_cast<float>(acos(-1.0));
const float PI_2 = static_cast<float>(2 * PI);

const GLuint EMPTY  = 0;

using Vec4 = glm::vec4;
using Mat4 = glm::mat4;

using Vec3 = glm::vec3;
using Mat3 = glm::mat3;

using Vec2 = glm::vec2;
using Mat2 = glm::mat2;


using String = std::string;

using IStream = std::istream;
using OStream = std::ostream;

using IFStream = std::ifstream;
using OFStream = std::ofstream;