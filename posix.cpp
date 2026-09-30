#include "header.h"
// for getpwuid() and struct passwd
using namespace std;

string home_dir;
string prev_dir;
string op;

vector<std::string> cmd_history;  

string formatPath(const string &cwds, const string &home_dir) {
    std::string op;
    if (cwds == home_dir) {
        op = "~";
    } 
    else if (cwds.rfind(home_dir + "/", 0) == 0) {
        // if cwd starts with home_dir + "/"
        op = "~" + cwds.substr(home_dir.size());
    } 
    else {
        op = cwds;
    }
    return op;
}


int is_amp(const vector<string>& tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "&" || tokens[i] == "|") {
            return i; 
        }
    }
    return -1; 

}

string cd(string cur_path,vector<string>&args)
{
    char buf[1024];
    string target;

    if (args.size() > 2) {
        cout << "Invalid arguments" << endl;
        return formatPath(cur_path, home_dir);   // stay put, prompt shows the unchanged dir
    }

    if (args.size() == 1) {

        target =home_dir;
    }
    else if (args[1] == "-") {

        target= prev_dir;
    }
    else if (args[1] == "~") {
        target =home_dir;
    }
     else {
        // covers ".", "..", and any relative/absolute path -- chdir() below
        // does the real work, so ".." doesn't need its own special case
        // (it used to, and that special case never actually called chdir()
        // when already at home_dir, so "cd .." from ~ did nothing at all)
        target = args[1];
        }

    string currentDir = cur_path;

    //  changing directory
    if (chdir(target.c_str()) != 0) {
        perror("cd failed");
        return formatPath(cur_path, home_dir);   // failed: keep showing the old dir, not the bad target
    } else {
        prev_dir = currentDir;   // update previous dir only if success

        // After successful cd, print the new directory
        getcwd(buf, sizeof(buf)) ;

            string cwds = string(buf);
            target=formatPath(cwds,home_dir);

        }

    return target;
}
    




void echo( vector<string>&token)
{
    int n=token.size();
    for (int i=1;i<n;i++)
    {
        cout<<token[i]<<" ";
    }
    cout<<endl;
}
//

// Runs exactly one command (everything main() used to do per readline() call).
// Pulled out into its own function so a single input line can be split on
// ';' and each piece run through the same dispatch logic. Returns false to
// mean "the user typed exit", which tells main() to stop the shell.
bool execute_line(const string &line)
{
    char cwd1[1024];
    getcwd(cwd1, sizeof(cwd1));
    string cwds = string(cwd1);

    vector<string> args = tokenize(line);
    if (args.empty()) return true;

    if (args[0] == "exit") return false;

    int pipe_pos = if_pipe(args);   // was "int pos = if_pipe(args) > 0" -- that
    if (pipe_pos > 0)               // stored the boolean, not the real index
    {
        pipes(args,pipe_pos);
    }
    else if(if_I_O(args)>0)
    {
        I_O(args);
    }
    else if(is_amp(args)>0)
    {
       foreground(line);
    }
    else if(args[0]=="cd")
    {
       op= cd(cwds,args);
    }

    else if(args[0]=="echo")
    {
      echo(args);
    }
    else if(args[0]=="pwd")
    {
        pwd();
    }
    else if((args[0]=="ls"))
    {
        ls(args);
    }
    else if(args[0]=="pinfo")
    {
        pinfo(args);
    }
    else if(args[0]=="history")
    {
        print_history(args);
    }
     else if(args[0]=="search")
    {
        if(args.size()!=2)   // covers both too many args AND a missing one
        {                     // (used to be args.size()>2, then read args[1]
            cout<<"arguments mismatch"<<endl;   // unconditionally below -- UB
        }                                        // when search was called bare)
        else if(search(".",args[1]))
        {
            cout<<"True"<<endl;
        }
        else cout<<"False"<<endl;
    }
    else{
        foreground(line);
    }

    return true;
}

int main() {
   
     char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    home_dir = string(cwd);
    prev_dir=formatPath(cwd,home_dir);

    signal(SIGINT, ctrl_c_z);  // for Ctrl+C if given
    signal(SIGTSTP, ctrl_c_z); // for Ctrl+Z if given

    
    struct passwd *user_info = getpwuid(getuid());//for password file
    if (user_info == NULL) {
        perror("getpwuid failed");
        return 1;
    }

    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) != 0) {  
        perror("gethostname"); //   error message 
        return 1; 
    }
    char cwd1[1024];
    getcwd(cwd1, sizeof(cwd1));
    string cwds = string(cwd1);
   
    op=formatPath(cwds,home_dir);

    load_his();

   
    while(1){

    string prompt=string(user_info->pw_name) + "@" + hostname + ":" + op + ">";
    
    char *c=readline(prompt.c_str());
     if (c==NULL)
     {
        
        break;
     } 
     if(strlen(c)==0)
     {
        

        continue;
     }

     //--------------------------------------ading to history----------------------
      string line(c);

     if(strlen(c)>0)
     {
          add_history(c);
        if (cmd_history.size() == 20) {
            cmd_history.erase(cmd_history.begin());
        }
        cmd_history.push_back(line);
     }
     
    
     free(c);
     //-------------------------------------history-----------------------

     //----------------- semicolon separated list of commands -------------
     bool keep_running = true;
     for (string segment : split_semicolons(line))
     {
        if (!execute_line(segment))
        {
            keep_running = false;   // "exit" was one of the ';'-separated commands
            break;
        }
     }
     if (!keep_running) break;
    //-----------------------------------------------------------------

    // cout << user_info->pw_name<< "@" << hostname << ":" << op << ">";

     save_history();

    }
    
    return 0;
}