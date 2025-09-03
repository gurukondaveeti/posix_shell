#include "header.h"


int if_I_O(const vector<string>& tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == ">" || tokens[i] == "<" || tokens[i] == ">>" || tokens[i] == "|") {
            return i; 
        }
    }
    return -1; 

}
void I_O( vector<string>& args)
{
    vector<std::string> cmd;
    string input_file;
    string output_file;
    bool append = false;

   for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "<") 
        {
            input_file = args[i+1];
            i++;
        }

        else if (args[i] == ">") 
        {
            output_file = args[i+1];
            i++;
        }
        else if (args[i] == ">>")
         {
            output_file = args[i+1];
            i++;
            append = true;
        }
        else 
        {
            cmd.push_back(args[i]);
        }

    }


     int pid = fork();

    if (pid == 0) {
        // --- Child ----------------

        // "<"  Input redirection 
        if (!input_file.empty()) {
            int fd = open(input_file.c_str(), O_RDONLY);
            if (fd < 0) {
                perror("while opening input");
                exit(EXIT_FAILURE);
            }
            dup2(fd, 0);
            close(fd);
        }

        // ">"  Output redirection 
        if (!output_file.empty()) {
            int fd;
            if(!append)
            {
             fd = open(output_file.c_str(),
                          O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            else
            {
              fd = open(output_file.c_str(),
                          O_WRONLY | O_CREAT | O_APPEND, 0644);  //Append
            }
            if (fd < 0) {
                perror("while opening output");
                exit(EXIT_FAILURE);
            }
            dup2(fd, 1);
            close(fd);
        }

         string line="";
            for(auto i :cmd)
            {
                line+=i+" ";
            }

        if(if_I_O(cmd)>0)
        {
            I_O(cmd);
        }
        else if(is_amp(cmd)>0)
        {
           foreground(line);
        }
         
        else if(cmd[0]=="echo")
        {
          echo(cmd);
        }
        else if(cmd[0]=="pwd")
        {
            pwd();
        }
        else if((cmd[0]=="ls"))
        {
            ls(cmd);
        }
        else if(cmd[0]=="pinfo")
        {
            pinfo(cmd);
        }
        else{
           
            foreground(line);
        }
        exit(EXIT_SUCCESS);
      
    } 
    else if (pid > 0) {
        // --- Parent process ---
        int status;
        waitpid(pid, &status, 0);
    } 
    else {
        perror("fork failed");
    }
}
