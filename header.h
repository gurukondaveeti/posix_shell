#include <stdio.h>
#include<bits/stdc++.h>
#include <iostream>
#include <cstring> 
#include <algorithm>
#include <fcntl.h>
#include <string> 
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <dirent.h>
#include <grp.h> 
#include <ctime>
#include <unistd.h>     // for getuid()
#include <sys/types.h>  // for uid_t
#include <pwd.h>        // for getpwuid() and struct passwd
using namespace std;

extern std::string home_dir;
extern std::string prev_dir;
extern std::vector<std::string> cmd_history;

void pwd();

string formatPath(const string &cwds, const string &home_dir);

vector<string> tokenize(const string &line);

string cd(string cur_path,vector<string>&args);

void ls(const vector<string>& args);

void foreground(const string &line);

vector<string> tokenize(const string &line);

int if_I_O(const std::vector<std::string>& tokens);

void I_O( vector<string>& args);

void echo(vector<string>&token);

void pinfo(vector<std::string>& args);
void print_history(vector<std::string>& args);
void load_his();
void save_history();
