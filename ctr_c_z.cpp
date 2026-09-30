#include "header.h"
vector <char *> str_to_cptr(vector<string> & args)
{
    vector <char *> c_ptr;
    for(int i=0;i<args.size();i++)
    {
        c_ptr.push_back(const_cast<char*> (args[i].c_str()));
    }
    c_ptr.push_back(NULL);

return c_ptr;
}

void ctrl_c_z(int signal)
{
if(signal==SIGINT){
    if(!fore_ground_pid)// if pid=0 then no foreground process
    {
        return;
    }
    kill(fore_ground_pid,SIGINT);
    }
if(signal==SIGTSTP)
{
     if(!fore_ground_pid)
    {
        return;
    }
    cout<<endl<<fore_ground_pid<<endl;
    kill(fore_ground_pid,SIGTSTP);
    fore_ground_pid=0;
    }

    cout<<endl;
}
