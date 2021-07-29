#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <v8.h>
using namespace std;
using namespace v8;

const char* ZendaVersion = "1.0.1";

const char* ZendaCreator = R"""(
 ______               _         ___ _____ 
|___  /              | |       |_  /  ___|
   / /  ___ _ __   __| | __ _    | \ `--.    | By Rodolfo Herrera Hernandez  
  / /  / _ \ '_ \ / _` |/ _` |   | |`--. \   | Full Stack Developer
./ /__|  __/ | | | (_| | (_| /\__/ /\__/ /   | https://github.com/rodiihernandezz/
\_____/\___|_| |_|\__,_|\__,_\____/\____/         
)""";

const char* OperativeSystem(){
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        #ifdef _WIN64
            return "WIN64"
        #else
            return "WIN32"
        #endif
    #elif __APPLE__
        return "APPLE";
    #elif __linux__
        return "LINUX";
    #elif __unix__
        return "UNIX";
    #elif defined(_POSIX_VERSION)
        return "POSIX";
    #else
        return "UKNOWN";
    #endif
}

const char* StringToLower(string String){
    transform(String.begin(), String.end(), String.begin(), ::tolower);
    return String.c_str();
}

const char* StringToUpper(string String){
    transform(String.begin(), String.end(), String.begin(), ::toupper);
    return String.c_str();
}

const char* ToCString(const String::Utf8Value& value) {
    return *value ? *value : "failed TOCSTRING.";
}

string StringStartAt(string String, unsigned int Position){
    string Buffer;

    while(String[Position])
        Buffer += String[Position], Position++;

    return Buffer;
}

string StringEndAt(string String, unsigned int Position){
    string Buffer;

    while(String[Position])
        Buffer += String[Position], Position--;

    return Buffer;
}

string StringStartEndAt(string String, unsigned int StartPosition, unsigned int EndPosition){
    string Buffer;

    while( (String[StartPosition]) && (StartPosition <= EndPosition) )
        Buffer += String[StartPosition], StartPosition++;

    return Buffer;
}

string StringLeftTrim(const string &String, string TrimCharacters = " \n\r\t\f\v"){
	size_t Start = String.find_first_not_of(TrimCharacters);

	return (Start == string::npos) ? "" : String.substr(Start);
}

string StringRightTrim(const string &String, string TrimCharacters = " \n\r\t\f\v"){
	size_t End = String.find_last_not_of(TrimCharacters);

	return (End == string::npos) ? "" : String.substr(0, End + 1);
}

string StringTrim(const string &String, string LeftTrimCharacters = " \n\r\t\f\v", string RightTrimCharacters = " \n\r\t\f\v"){
	return StringRightTrim(StringLeftTrim(String, LeftTrimCharacters), RightTrimCharacters);
}

vector<string> SplitString(const string &str, const string &delim){
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do{
        pos = str.find(delim, prev);
        if (pos == string::npos)
            pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty())
            tokens.push_back(StringTrim(token));
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

const char* ConsoleBackground(string Background){
    map<string, string> Backgrounds;
 
    map<string, string>::iterator BackgroundsIterator;
 
    typedef pair<string, string> Pair;
 
    Backgrounds.insert(Pair("black", "\33[40m"));
    Backgrounds.insert(Pair("red", "\33[41m"));
    Backgrounds.insert(Pair("green", "\33[42m"));
    Backgrounds.insert(Pair("yellow", "\33[43m"));
    Backgrounds.insert(Pair("blue", "\33[44m"));
    Backgrounds.insert(Pair("violet", "\33[45m"));
    Backgrounds.insert(Pair("beige", "\33[46m"));
    Backgrounds.insert(Pair("white", "\33[47m"));
 
    BackgroundsIterator = Backgrounds.find(
        StringToLower(Background)
    );
    
    return BackgroundsIterator != Backgrounds.end() ? BackgroundsIterator->second.c_str() :  "";
}

const char* ConsoleStyle(string Style){
    map<string, string> Styles;

    map<string, string>::iterator StylesIterator;

    typedef pair<string, string> Pair;

    Styles.insert(Pair("normal", "\33[0m"));
    Styles.insert(Pair("end", "\n"));
    Styles.insert(Pair("bold", "\33[1m"));
    Styles.insert(Pair("italic", "\33[3m"));
    Styles.insert(Pair("url", "\33[4m"));
    Styles.insert(Pair("blink", "\33[5m"));
    Styles.insert(Pair("blink2", "\33[6m"));
    Styles.insert(Pair("selected", "\33[7m"));

    StylesIterator = Styles.find(
        StringToLower(Style)
    );

    return StylesIterator != Styles.end() ? StylesIterator->second.c_str() :  "";
}

const char* ConsoleColor(string Color){
    map<string, string> Colors;

    map<string, string>::iterator ColorsIterator;

    typedef pair<string, string> Pair;

    Colors.insert(Pair("black", "\33[30m"));
    Colors.insert(Pair("red", "\33[31m"));
    Colors.insert(Pair("green", "\33[32m"));
    Colors.insert(Pair("yellow", "\33[33m"));
    Colors.insert(Pair("blue", "\33[34m"));
    Colors.insert(Pair("violet", "\33[35m"));
    Colors.insert(Pair("beige", "\33[36m"));
    Colors.insert(Pair("white", "\33[37m"));
    Colors.insert(Pair("grey", "\33[90m"));

    ColorsIterator = Colors.find(
        StringToLower(Color)
    );

    return ColorsIterator != Colors.end() ? ColorsIterator->second.c_str() :  "";
}

void ClearConsole(){
    string OS = OperativeSystem();
    (OS == "WIN32") || (OS == "WIN64") ? system("cls") : system("clear");
}

void PrintVersion(){
    cout << "ZendaJS v" << ZendaVersion << endl;
}

void PrintCreator(){
    cout << ZendaCreator << endl;
}