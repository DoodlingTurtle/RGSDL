#include "../Utils.h"

namespace RGSDL::Utils {

#define contbreak_ws( ch ) if ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' )

  bool readIni( const std::string& filename, IniType& fillin )
  {
    std::ifstream file;
    file.open( filename.c_str(), std::ifstream::in );

    if(!file.good()) { Error("Could not find ini file '" + filename + "'"); return false; }

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
          contbreak_ws( ch )
          {
            continue;
          }
          else if ( ch == '[' )
          {
            readmode = 1;
          }
          else
          {
            Error( "ini files must start with a '[...]' - Group" );
            readmode = 99;
          }
          break;

        case 1:
          if ( ch != ']' ) {
            groupName += ch;
          }
          else {
            readmode = 2;
            fillin.emplace( groupName, std::unordered_map<std::string, std::string>() );
          }
          break;

        case 2:
          contbreak_ws( ch )
          {
            continue;
          }
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
          contbreak_ws( ch )
          {
            whitespace += ch;
          }
          else if ( ch == '=' )
          {
            readmode = 4;
          }
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
          else
          {
            readmode = 5;
          }

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

  IniGroup readIniGroup( IniType& ini, std::string group )
  {
    auto fnd = ini.find( group );
    if ( fnd == ini.end() )
      return IniGroup();
    else
      return ini[ group ];
  }

  std::string readIniGroupValue( IniGroup& grp, std::string key, std::string def )
  {
    if ( grp.find( key ) == grp.end() )
      return def;
    else
      return grp[ key ];
  }

} // namespace RGSDL::Utils