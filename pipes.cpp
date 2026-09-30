#include "header.h"

int if_pipe(const vector<string>& tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if ( tokens[i] == "|") {
            return i; 
        }
    }
    return -1; 

}

void pipes(const vector<string> &args,int pos)
{
    vector<string> left_cmd;
    vector<string> right_cmd;
    vector<char *>left_ch;
    vector<char *>right_ch;
    
    int i=0;
    while(i<pos)
    {
        left_cmd.push_back(args[i]);
        i++;
    }
    i++;
    while(i<args.size())
    {
        right_cmd.push_back(args[i]);
        i++;
    }
    left_ch=str_to_cptr(left_cmd);
    right_ch=str_to_cptr(right_cmd);

    int fd[2];
    pipe(fd);
    if (fork() != 0) { //if  parent 
        close(fd[0]); // Closing the read end of the pipe
        dup2(fd[1], STDOUT_FILENO); // Redirect standard output to the write end of the pipe
        close(fd[1]); // Closing the write end of the pipe

         if(!args.empty())
            {
                if(execvp(left_ch[0],left_ch.data())<0)
                {
                    perror("execvp failed");
                       exit(1);
                }
            }

        
        
        
    } else {
        close(fd[1]); // Close the write end of the pipe
        dup2(fd[0], STDIN_FILENO); // Redirect standard input to the read end of the pipe
        close(fd[0]); // Close the read end of the pipe

         if(!args.empty())
            {
                if(execvp(right_ch[0],right_ch.data())<0)
                {
                    perror("execvp failed");
                       exit(1);
                }
            }
        
        
}

}