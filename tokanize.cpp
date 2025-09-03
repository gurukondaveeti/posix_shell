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