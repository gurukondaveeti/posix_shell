#include "header.h"



struct p_info{
   
    string status;
    int pgrp;
    string vm_size;
};

string is_fore(int pgrp)
{
    int terminal_pgrp = tcgetpgrp(STDIN_FILENO);
    string plus = (pgrp == terminal_pgrp) ? "+" : "";
    return plus;
}


void pinfo(vector<std::string>& args)
{
    p_info res;
    int pid;
 if(args.size()==1)
 {
    pid=getpid();
 }
 else{
    pid=stoi(args[1]);
 }
  string value;
string stat_path = "/proc/" + to_string(pid) + "/stat";
ifstream stat_file(stat_path);

bool valid=stat_file.is_open();
if (!valid) {
        cout << "process with PID : " << pid << " not found." << endl;
        return;
    }
    int pgrp ;
    string line;
 if (getline(stat_file, line)) {
        
       
        char* buffer = new char[line.length() + 1];
        strcpy(buffer, line.c_str());

        int i = 1;
        
       
        char* token = strtok(buffer, " ");//first token

        // Looping  the rest of the tokens
        while (token != NULL) {
            
            if (i == 3) {  // 3rd field is process state
                res.status = token[0];
            }
            if (i == 23) {  // 23rd field is virtual memory size
                res.vm_size=token;
            }
            if (i == 5)  pgrp = stoi(token);//process group ID
            token = strtok(NULL, " ");//moving to next token
            ++i;
        }

        //  Clean up the allocated memory
        delete[] buffer;
    }
    else{
        cout<<"stat_file is empty"<<endl;
        return;
    }


string exe_path_link = "/proc/" + to_string(pid) + "/exe";
char exe_path[1024];

int len = readlink(exe_path_link.c_str(), exe_path, sizeof(exe_path) - 1);//no of bytes read
    if (len != -1) {
        exe_path[len] = '\0';
    } else {
        strcpy(exe_path, "Path not accessible");
    }

res.status=res.status+is_fore(pgrp);


std::cout << "Process Status -- {" << res.status << "}" << std::endl;
    std::cout << "memory -- " << res.vm_size << " {Virtual Memory}" << std::endl;
    std::cout << "Executable Path -- " << exe_path << std::endl;

}


void pwd()
{
  char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    string cwds = string(cwd);
    cout<<cwds<<endl;
   
}