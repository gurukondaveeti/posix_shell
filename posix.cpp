#include "header.h"
// for getpwuid() and struct passwd
using namespace std;

string home_dir;
string prev_dir;

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


vector<string> tokenize(const string &line) {
    vector<string> tokens;
    // Make a modifiable copy of input (strtok needs char*)
    char* cstr = new char[line.size() + 1];
    strcpy(cstr, line.c_str());


    char* token = strtok(cstr, " \t");  // split on spaces & tabs

    while (token != nullptr) {
        tokens.push_back(string(token));
        token = strtok(nullptr, " \t"); // keep splitting
    }

    delete[] cstr;

    return tokens;
}

string cd(string cur_path,vector<string>&args)
{
    char buf[200];
    string target;

    if (args.size() == 1) {
       
        target =home_dir;
    } 
    else if (args[1] == "-") {   
         
        target= prev_dir;
    } 
    else if (args[1] == "~") {   
        target =home_dir;
    } 
    else if(args[1] == "..")
    {
        if(cur_path==home_dir)
        {
            prev_dir = cur_path; 
            return "~";
        }
        else {
        target = args[1]; 
        }
    }
     else {
        target = args[1]; 
        }
   
    string currentDir = cur_path;

    // Try to change directory
    if (chdir(target.c_str()) != 0) {
        perror("cd failed");
    } else {
        prev_dir = currentDir;   // update previous dir only if success

        // After successful cd, print the new directory
        getcwd(buf, sizeof(buf)) ;
            
            string cwds = string(buf);
            target=formatPath(cwds,home_dir);
            
        }
    
    return target;
}
    







void echo(vector<string>&token)
{
    int n=token.size();
    for (int i=1;i<n;i++)
    {
        cout<<token[i]<<" ";
    }
    cout<<endl;
}


int main() {
   
     char cwd[200];
    getcwd(cwd, sizeof(cwd));
    home_dir = string(cwd);
    prev_dir=formatPath(cwd,home_dir);

    


    // Retrieve the password file entry for this user ID.
    struct passwd *user_info = getpwuid(getuid());
    if (user_info == NULL) {
        perror("getpwuid failed");
        return 1;
    }

    char hostname[200];
    if (gethostname(hostname, sizeof(hostname)) != 0) {  
        perror("gethostname"); // perror() prints the error message for the last failed function call.
        return 1; // Return a non-zero value to indicate an error
    }
     string op;

    char cwd1[200];
    getcwd(cwd1, sizeof(cwd1));
    string cwds = string(cwd1);
   
      op=formatPath(cwds,home_dir);

    cout << user_info->pw_name<< "@" << hostname << ":" << op << ">";

    while(1){

    string line;

     char cwd1[200];
    getcwd(cwd1, sizeof(cwd1));
    string cwds = string(cwd1);
   
    op=formatPath(cwds,home_dir);

    
    

     if (!getline(cin, line))
     {
        cout<<endl;
        break;
     } 
     vector<string> args = tokenize(line);
        if (args.empty()) continue;

        if (args[0] == "exit") break;
        else if(args[0]=="cd")
        {
           op= cd(cwds,args);
        }
        else if(args[0]=="echo")
        {
          echo(args);
        }
        else if((args[0]=="ls"))
        {
            ls(args);
        }
    
        

    cout << user_info->pw_name<< "@" << hostname << ":" << op << ">";

        
    
    }    
    
    return 0;
}