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