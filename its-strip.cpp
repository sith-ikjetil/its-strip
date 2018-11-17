//
// Written by Kjetil Kristoffer Solberg
//
// (i) Filters away chars from input
//

//
// #include
//
#include <vector>
#include <iostream>
#include <string>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "itsoftware-linux-core.h"
#include "itsoftware-linux.h"

//
// using
//
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

//
// struct
//
// (i) Contains command line arguments spported by this application
//
struct ItsStripArguments
{
    string Hex;
    string Des;
    string Chars;
    string Columns;
    string FileIn;
    string FileOut;
    vector<int> StripChars;
    vector<int> StripColumns;
    bool IsHelp;
    bool IsInverse;

    bool IsEmpty()
    {
        return (StripChars.size() == 0 && Columns.size() == 0 && !IsHelp && FileIn.size() == 0 && FileOut.size() == 0 );
    }
};


//
// Function Prototypes
//
void PrintHelp();
void PrintHelp(string msg);
int ExecuteStrip(ItsStripArguments& args);
int ParseArguments(int argc, char** argv, ItsStripArguments& args);
bool CheckForStripStatus(int c, ItsStripArguments& args);

//
// Function: main
//
// (i) Main
//
int main(int argc, char** argv)
{
    ItsStripArguments args;

    if ( ParseArguments(argc, argv, args) == EXIT_FAILURE )
    {
        stringstream ss;
        ss << "ParseArguments failed with code:" << errno;

        string msg = ss.str();
        PrintHelp(msg);
    
        return EXIT_FAILURE;
    }

    if ( (args.IsHelp || args.IsEmpty()) )
    {
        PrintHelp();
        return EXIT_SUCCESS;
    }

    if ( ExecuteStrip(args) == EXIT_FAILURE )
    {
        stringstream ss;
        ss << "ExceuteStrip failed with code: " << errno;

        string msg = ss.str();
        PrintHelp(msg);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//
// Function: PrintHelp
//
// (i) Prints Help and Usage and Message
//
void PrintHelp(string msg)
{
    PrintHelp();
    cout << endl;
    cout << "Message: " << msg << endl;
}

//
// Function: PrintHelp
//
// (iI Prints Help and Usage
//
void PrintHelp()
{
    cout << "Usage: its-strip [OPTION]..." << endl;
    cout << "   or: its-strip [OPTION]... [FILE IN] [FILE OUT]" << endl;
    cout << endl;
    cout << "With no FILE, read standard input" << endl;
    cout << endl;
    cout << "  -?, --help            Shows this help screen" << endl;
    cout << "  -h, --hex             Strip all these hex values away. Example: -h \"0D48\"" << endl;
    cout << "  -d, --des             Strip all these numeric values away. Example: -d \"13 48\"" << endl;
    cout << "  -c, --chars           Strip all these chars away. Example: -c \"abcdef\"" << endl;
    cout << "  -l, --columns         Strip columns separated by space. Not combinable. Example: -l \"1 4 5\"" << endl;
    cout << "  -i, --inverse         Inverses strip. Not columns." << endl;
    cout << endl;
    cout << "Example:" << endl;
    cout << "  Removes all t, i and space from input:" << endl; 
    cout << "    ./its-strip -c \"ti \"" << endl;
}

//
// Function: ParseArguments
//
// (i) Parses the command line arguments and populates the ItsStripArguments structure.
//
int ParseArguments(int argc, char** argv, ItsStripArguments& args)
{
    bool bIsHex = false;
    bool bIsDes = false;
    bool bIsChars = false;
    bool bIsColumns = false;
    bool bFileIn = true;
    bool bFileOut = false;

    args.IsHelp = false;

    for ( int i = 1; i < argc; i++)
    {
        string str(argv[i]);
        if ( str == "-?" || str == "--help" )
        {
            args.IsHelp = true;
            return EXIT_SUCCESS;
        }

        if ( str == "-i" || str == "--inverse" )
        {
            args.IsInverse = true;
            continue;
        }

        if ( !bIsHex )
        {
            if ( str == "-h" || str == "--hex" )
            {
              bIsHex = true;
              continue;
            }
        }
        if ( bIsHex )
        {
          args.Hex = argv[i];
          bIsHex = false;
          continue;
        }

        if ( !bIsDes )
        {
            if ( str == "-d" || str == "--des" )
            {
              bIsDes = true;
              continue;
            }
        }
        if ( bIsDes )
        {
          args.Des = argv[i];
          bIsDes = false;
          continue;
        }

        if ( !bIsChars )
        {
            if ( str == "-c" || str == "--chars" )
            {
              bIsChars = true;
              continue;
            }
        }
        if ( bIsChars )
        {
          args.Chars = argv[i];
          bIsChars = false;
          continue;
        }

        if ( !bIsColumns )
        {
            if ( str == "-l" || str == "--columns" )
            {
                bIsColumns = true;
                continue;
            }
        }
        else if ( bIsColumns )
        {
            args.Columns = argv[i];
            bIsColumns = false;
            continue;
        }

        if ( bFileIn )
        {
            bFileIn = false;
            bFileOut = true;
            args.FileIn = argv[i];
            continue;
        }

        if ( bFileOut )
        {
            bFileIn = false;
            bFileOut = false;
            args.FileOut = argv[i];
            break;
        }
    }

    //
    // StripChars extraction
    //
    if ( args.Hex.size() > 0 )
    {
        char b[3];
        for ( int i = 0; i < args.Hex.size(); i+=2 )
        {
            b[0] = args.Hex[i];
            b[1] = args.Hex[i+1];
            b[2] = '\0';
            args.StripChars.push_back(ItSoftware::Linux::ItsConvert::ToIntFromHex(b));
        }
    }

    if ( args.Des.size() > 0 )
    {
        args.StripChars.push_back(ItSoftware::Linux::ItsConvert::ToInt(args.Des));
    }

    if ( args.Chars.size() > 0 )
    {
        for ( int i = 0; i < args.Chars.size(); i++ )
        {
            args.StripChars.push_back(static_cast<int>(args.Chars.at(i)));
        }
    }

    //
    // Columns extraction
    //
    if ( args.Columns.size() > 0 )
    {
        auto c = ItSoftware::Linux::ItsString::Split(args.Columns," ");
        for ( int i = 0; i < c.size(); i++ )
        {
            args.Columns.push_back(ItSoftware::Linux::ItsConvert::ToInt(c.at(i)));
        }
    }

    return EXIT_SUCCESS;
}

//
// Function: ExecuteStrip
//
// (i) Execute Strip Function
//
int ExecuteStrip(ItsStripArguments& args)
{
    //
    // Setup Read Stream
    //
    FILE* readStream = stdin;
    if ( args.FileIn.size() > 0 )
    {
        readStream = fopen(args.FileIn.c_str(), "r");
        if ( readStream == nullptr )
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        readStream = stdin;
        if ( readStream == nullptr)
        {
            return EXIT_FAILURE;
        }
    }

    //
    // Setup Write Stream
    //
    FILE* writeStream = stdout;
    if ( args.FileOut.size() > 0 )
    {
        writeStream = fopen(args.FileOut.c_str(),"w");
        if ( writeStream == nullptr )
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        writeStream = stdout;
        if ( writeStream == nullptr)
        {
            return EXIT_FAILURE;
        }
    }

    int c;
    while ((c = fgetc (readStream)) != EOF)
    {
        if ( !CheckForStripStatus(c, args) )
        {
            fputc(c,writeStream);
        }
    }

    fclose(readStream);
    fclose(writeStream);

    return EXIT_SUCCESS;
}

//
// Function: CheckForStripStatus
//
// (i) Checks given character if it should be stripped or not.
//
bool CheckForStripStatus(int c, ItsStripArguments& args)
{
    if ( args.IsInverse )
    {
        for ( int i = 0; i < args.StripChars.size(); i++ )
        {
            if ( c == args.StripChars.at(i) )
            {
                return false;
            }
        }
        return true;
    }

    if ( args.Columns.size() > 0 )
    {
        static bool firstRun = false;
        static int column = 1;
        static bool bHit = false;
        static bool bLastWasSpace = false;

        if ( c == '\n' )
        {
            firstRun = false;
            column = 1;
            bHit = false;

            return false;
        }

        if ( !bHit && !firstRun)
        {
            firstRun = true;
            for ( int i = 0; i < args.Columns.size(); i++ )
            {
                if ( args.Columns.at(i) == column )
                {
                    bHit = true;
                    break;
                }
            }
        }

        if ( c == ' ' && bLastWasSpace == false)
        {
            bLastWasSpace = true;
            bHit = false;
            column++;

            for ( int i = 0; i < args.Columns.size(); i++ )
            {
                if ( args.Columns.at(i) == column )
                {
                    bHit = true;
                    break;
                }
            }

            if ( bHit ) 
            {
                return true;
            }
        }
        else if ( bHit ) 
        {
            bLastWasSpace = false;
            return true;
        }

        return false;
    }
    else 
    {
        for ( int i = 0; i < args.StripChars.size(); i++ )
        {
            if ( c == args.StripChars.at(i) )
            {
                return true;
            }
        }
    
        return false;
    }

    return false;
}