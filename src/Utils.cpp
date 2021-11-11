#include "../Utils.h"
#include <memory>

namespace RGSDL::Utils {

#define contbreak_ws( ch ) if ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' )

    bool readIni( const std::string& filename, IniType& fillin )
    {
        std::ifstream file;
        file.open( filename.c_str(), std::ifstream::in );

        if ( !file.good() ) {
            Error( "Could not find ini file '" + filename + "'" );
            return false;
        }

        unsigned char readmode      = 0;
        unsigned char prev_readmode = 0;

        std::string groupName  = "";
        std::string property   = "";
        std::string whitespace = "";
        char        ch;
        char        buffer[ 1024 ];

        while ( !file.eof() && readmode != 99 ) {
            ch = file.get();
            switch ( readmode ) {
                case 0:
                    contbreak_ws( ch ) { continue; }
                    else if ( ch == '[' ) { readmode = 1; }
                    else
                    {
                        Error( "ini files must start with a '[...]' - Group" );
                        readmode = 99;
                    }
                    break;

                case 1:
                    if ( ch != ']' ) { groupName += ch; }
                    else {
                        readmode = 2;
                        fillin.emplace( groupName, std::unordered_map<std::string, std::string>() );
                    }
                    break;

                case 2:
                    contbreak_ws( ch ) { continue; }
                    else if ( ch == '[' && prev_readmode == 5 )
                    {
                        groupName = "";
                        readmode  = 1;
                    }
                    else
                    {
                        property += ch;
                        readmode      = 3;
                        prev_readmode = 2;
                    }
                    break;

                case 3:
                    contbreak_ws( ch ) { whitespace += ch; }
                    else if ( ch == '=' ) { readmode = 4; }
                    else
                    {
                        property += whitespace + ch;
                        whitespace = "";
                    }
                    break;

                case 4:
                    contbreak_ws( ch )
                    {
                        continue;
                        break;
                    }
                    else { readmode = 5; }

                case 5:
                    file.getline( buffer, 1024 );
                    fillin[ groupName ].emplace( property, ch + std::string( buffer ) );

                    property      = "";
                    whitespace    = "";
                    prev_readmode = 5;
                    readmode      = 2;

                    break;
            }
        }

        file.close();
        return true;
    }

    IniGroup readIniGroup( const IniType& ini, const std::string& group )
    {
        auto fnd = ini.find( group );
        if ( fnd == ini.end() ) return IniGroup();
        else
            return fnd->second;
    }

    std::string
        readIniGroupValue( const IniGroup& grp, const std::string& key, const std::string def )
    {
        auto fnd = grp.find( key );
        if ( fnd == grp.end() ) return def;
        else
            return fnd->second;
    }

    int readIniGroupInt( const IniGroup& grp, const std::string& key, const int def )
    {
        auto fnd = grp.find( key );
        if ( fnd == grp.end() ) return def;
        else
            return atoi( fnd->second.c_str() );
    }

    std::string stringTrim( const std::string& src )
    {
        if ( src.length() == 0 ) return "";
        size_t start = 0;
        size_t end   = src.length() - 1;

        size_t newStart = start;
        size_t newEnd   = end;

        char ch = src.at( start );
        while ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' ) {
            try {
                newStart++;
                ch = src.at( newStart );
            }
            catch ( std::out_of_range* e ) {
                break;
            }
        }
        start = newStart;

        ch = src.at( end );
        while ( newEnd >= start && ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' ) ) {
            try {
                newEnd--;
                ch = src.at( newEnd );
            }
            catch ( std::out_of_range* e ) {
                break;
            }
        }

        return src.substr( newStart, ( newEnd - newStart + 1 ) );
    }

    int stringSplit(
        const std::string& src, const std::string& delimiter, std::vector<std::string>& fillin,
        bool trimResult, int maxsplits )
    {
        if ( maxsplits < 2 ) {
            if ( trimResult ) fillin.push_back( stringTrim( src ) );
            else
                fillin.push_back( "" + src );
            return 1;
        }

        int splits = 1;

        size_t pos   = src.find( delimiter );
        size_t start = 0;

        while ( splits < maxsplits && pos != std::string::npos ) {
            if ( trimResult ) fillin.push_back( stringTrim( src.substr( start, pos - start ) ) );
            else
                fillin.push_back( src.substr( start, pos - start ) );
            start = pos + delimiter.length();
            pos   = src.find( delimiter, start );

            splits++;
        }

        std::string tmp = stringTrim( src.substr( start ) );
        if ( trimResult ) fillin.push_back( stringTrim( tmp ) );
        else
            fillin.push_back( tmp );

        return splits;
    }

    void exec( const std::string& command, std::vector<std::string>* fillin, bool trimLines )
    {
        char                                       buffer[ 256 ];
        std::unique_ptr<FILE, decltype( &pclose )> pipe( popen( command.c_str(), "r" ), pclose );
        if ( !pipe ) {
            Error( "popen() failed!" );
            return;
        }

        if ( trimLines )
            while ( fgets( buffer, 256, pipe.get() ) != nullptr )
                fillin->push_back( RGSDL::Utils::stringTrim( std::string( buffer ) ) );
        else
            while ( fgets( buffer, 256, pipe.get() ) != nullptr )
                fillin->push_back( std::string( buffer ) );
    }

    std::string stringJoin(const std::string& glue, const std::vector<std::string>& list) {
        if(list.size() == 0) return "";

        std::string ret = list[0];
        for(int a = 1; a < list.size(); a++)
            ret += glue + list[a];

        return ret;
    }

} // namespace RGSDL::Utils