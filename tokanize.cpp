#include "header.h"


vector<string> tokenize(const string &line) {
    vector<string> tokens;
   
    char* cstr = new char[line.size() + 1];
    strcpy(cstr, line.c_str());


    char* token = strtok(cstr, " \t");  
    while (token != nullptr) {
        tokens.push_back(string(token));
        token = strtok(nullptr, " \t"); 
    }

    delete[] cstr;

    return tokens;
}

// Splits one input line into the semicolon-separated commands it contains,
// e.g. "ls -a -l ; cd test" -> {"ls -a -l ", " cd test"}. Same strtok-on-a-
// copied-buffer approach as tokenize() above, just with ";" as the delimiter.
vector<string> split_semicolons(const string &line) {
    vector<string> commands;

    char* cstr = new char[line.size() + 1];
    strcpy(cstr, line.c_str());

    char* token = strtok(cstr, ";");
    while (token != nullptr) {
        commands.push_back(string(token));
        token = strtok(nullptr, ";");
    }

    delete[] cstr;

    return commands;
}