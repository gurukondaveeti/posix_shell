#include "header.h"





void foreground(const string &line)
{
    char* c_point_str = new char[line.size() + 1];
    strcpy(c_point_str, line.c_str());
    vector<char*>args;
    char* token= strtok(c_point_str," \t\n");

    while (token !=NULL)
    {
        args.push_back(token);
        token=strtok(NULL," \t\n");
    }
    
    string last=string(args.back());
    args.push_back(NULL);
    if(last!="&")//foreground
    {
        int pid;
        int waitp;
        if((pid=fork())<0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (pid>0)
        {
            waitpid(pid, &waitp, 0);
        }
        if (pid==0)
        {
            if(!args.empty())
            {
                execvp(args[0],args.data());
            }
        }
    }
    if(last=="&"){       //background process
            int pid;
        //char *bg_command="gedit";
///char *bg_command = (char*)"gedit";
//char *argv[] = { bg_command, NULL };  // argv[0] must be the program name, last must be NULL

args.pop_back();
args.pop_back();
args.push_back(NULL);


        /*int len=strlen(bg_command);
        if(len>0)
        {
            bg_command[len-1]='\0';
        }
        */
        if((pid=fork())<0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (pid>0)
        {
        
            cout<<pid<<endl;
        }
        
        if (pid==0)
        {
            if(!args.empty())
            {
                if(execvp(args[0],args.data())<0)
                {
                    perror("execvp failed");
                       exit(1);
                }
            }
        }
    }
}