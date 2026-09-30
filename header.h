#include <stdio.h>
#include<bits/stdc++.h>
#include <iostream>
#include <cstring> 
#include <algorithm>
#include <fcntl.h>
#include <string> 
#include <sys/stat.h>
#include <queue>
#include <signal.h>
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

extern string home_dir;
extern string prev_dir;
extern vector<string> cmd_history;
extern int fore_ground_pid;

bool search(const string& root, const string& target) ;

void pwd();

void ctrl_c_z(int signal);

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
int is_amp(const vector<string>& tokens);
void pipes(const vector<string> &args,int pos);
int if_pipe(const vector<string>& tokens) ;
vector <char *> str_to_cptr(vector<string> & args);