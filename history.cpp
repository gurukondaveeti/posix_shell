#include "header.h"
#include "header.h"


string his_filepath= ".history_file";

void save_history() {
    // Open in truncate mode to overwrite with the latest history
    ofstream history_file(his_filepath, std::ios::trunc);
    if (!history_file) {
        cout << "Error: Could not open history file for writing." << endl;
        return;
    }
    for ( auto& cmd : cmd_history) {
        history_file << cmd << endl;
    }
    history_file.close();
}



void load_his()
{
    
    ifstream his_file(his_filepath);
  
     for(string line;getline(his_file, line);) {
        if (!line.empty()) {
            cmd_history.push_back(line);
            add_history(line.c_str()); // Add to readline's internal history
        }
    }
    his_file.close();
}


void print_history(vector<std::string>& args)
{
    int pointer=cmd_history.size()-1;
    int start=0;
    if(args.size()==1)//print max 10
    {
        
        int count=10;
        while(pointer>=0&&count>=0)
        {
            cout<<" "<<cmd_history[start++]<<endl;
            count--;pointer--;
        }
    }
    if(args.size()==2)
    {
        int count=stoi(args[1]);
        int maxcount=20;
         while(maxcount>=0&&count>=0&&pointer)
        {
            cout<<" "<<cmd_history[start++]<<endl;
            count--;pointer--;
        }

    }
}