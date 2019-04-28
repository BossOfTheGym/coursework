#pragma once

#include <iostream>
#include <fstream>

#include <cmath>
#include <utility>
#include <thread>
#include <memory>
#include <limits>

#include <filesystem>
#include <functional>

#include <string>
#include <vector>
#include <map>


#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const double PI   = static_cast<double>(acos(-1.0));
const double PI_2 = static_cast<double>(2 * PI);


const double G = 6.6740831313131313e-11;
const double M = 5.9722e+24;             // earth mass
const double R = 6378000.0;                  // radius at equator

const GLuint EMPTY  = 0;


using UInt = unsigned int;


using Vec4 = glm::tvec4<double>;
using Mat4 = glm::tmat4x4<double>;

using Vec3 = glm::tvec3<double>;
using Mat3 = glm::tmat3x3<double>;

using Vec2 = glm::tvec2<double>;
using Mat2 = glm::tmat2x2<double>;


using String = std::string;

using IStream = std::istream;
using OStream = std::ostream;

using IFStream = std::ifstream;
using OFStream = std::ofstream;