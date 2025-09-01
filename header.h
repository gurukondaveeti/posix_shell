#include <stdio.h>
#include<bits/stdc++.h>
#include <iostream>
#include <cstring> 
#include <string> 
#include <dirent.h>
#include <grp.h> 
#include <ctime>
#include <unistd.h>     // for getuid()
#include <sys/types.h>  // for uid_t
#include <pwd.h>        // for getpwuid() and struct passwd
using namespace std;

extern std::string home_dir;
extern std::string prev_dir;


string formatPath(const string &cwds, const string &home_dir);

vector<string> tokenize(const string &line);

string cd(string cur_path,vector<string>&args);

void ls(const vector<string>& args);

