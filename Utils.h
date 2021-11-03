#pragma once

#ifndef PI
#define PI 3.141592653589793238462643383279
#endif  // PI

#ifndef PI2
#define PI2 6.283185307179586476925286766558
#endif  // PI2

#ifndef RandF
#define RandF() ((float)rand() / (float)RAND_MAX)
#endif

#include <iostream>
#define Error(msg) std::cout << "Error: " << msg << std::endl
#ifndef Debug
#ifdef DEBUG_BUILD
#define Debug(msg) std::cout << "Debug: " << msg << std::endl
#else
#define Debug(msg) /**/
#endif
#endif

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
namespace RGSDL::Utils {

typedef std::unordered_map<std::string, std::string> IniGroup;
typedef std::unordered_map<std::string, IniGroup> IniType;

bool readIni(const std::string &filename, IniType &fillin);

IniGroup readIniGroup(IniType &ini, std::string group);

std::string readIniGroupValue(IniGroup &grp, std::string key,
                              std::string def = std::string(""));

}  // namespace RGSDL::Utils