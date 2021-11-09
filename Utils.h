#pragma once

#ifndef PI
#define PI 3.141592653589793238462643383279
#endif // PI

#ifndef PI2
#define PI2 6.283185307179586476925286766558
#endif // PI2

#ifndef RandF
#define RandF() ( (float)rand() / (float)RAND_MAX )
#endif

#include <iostream>
#define Error( msg ) std::cout << "Error: " << msg << std::endl
#ifndef Debug
#ifdef DEBUG_BUILD
#define Debug( msg ) std::cout << "Debug: " << msg << std::endl
#else
#define Debug( msg ) /**/
#endif
#endif

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
namespace RGSDL::Utils {

    typedef std::unordered_map<std::string, std::string> IniGroup;
    typedef std::unordered_map<std::string, IniGroup>    IniType;

    bool readIni( const std::string& filename, IniType& fillin );

    IniGroup readIniGroup( const IniType& ini, const std::string& group );

    std::string readIniGroupValue( const IniGroup& grp, const std::string& key, const std::string def = "" );

    int readIniGroupInt( const IniGroup& grp, const std::string& key, const int def = 0 );

    std::string stringTrim( const std::string& src );

    int stringSplit(
        const std::string& src, const std::string& delimiter, std::vector<std::string>& fillin,
        bool trimResult = true, int maxsplits = std::numeric_limits<int>::max() );

    /** \brief executes `command` and writes all lines to output
     * 
     * \param command - a shell command
     * \param output  - will contain all lines (max length 256 byte per line) that were printed to std::out 
     * \param trimLines - if true, all linebreaks and whitespaces at the beginning and end of each line are removed
     */ 
    void exec(const std::string& command, std::vector<std::string>* output, bool trimLines = false);

} // namespace RGSDL::Utils