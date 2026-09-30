#include "header.h"


// Was a plain relative ".history_file" -- re-opened relative to whatever the
// CURRENT directory happened to be (save_history() runs after every command,
// so a single `cd` mid-session made it start writing somewhere else
// entirely). Anchoring it to $HOME makes "persists across sessions" actually
// true regardless of where the shell is launched from or cd'd to.
static string compute_history_path() {
    const char* h = getenv("HOME");
    if (h) return string(h) + "/.history_file";
    return ".history_file";   // fallback if $HOME is somehow unset
}

string his_filepath = compute_history_path();

void save_history() {
    // Open in truncate mode to overwrite with the latest history
    ofstream history_file(his_filepath, std::ios::trunc);
    if (!history_file) {
        cout << "not opening history file for writing." << endl;
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
    stack<string>s_h;
    if(args.size()==1)//print max 10
    {

        int count=9;   // was 10 -- "count>=0" below already runs 0..count
                       // inclusive, so starting at 10 printed 11 lines
        while(pointer>=0&&count>=0)
        {
            s_h.push(cmd_history[pointer]);
            count--;pointer--;
        }
    }
    if(args.size()==2)
    {
        bool numeric = !args[1].empty();
        for (char c : args[1]) {
            if (!isdigit((unsigned char)c)) { numeric = false; break; }
        }
        if (!numeric) {
            cout << "history: numeric argument required" << endl;
            return;
        }
        int count=stoi(args[1])-1;   // -1 to match the "0..count inclusive" loop below
        int maxcount=20;
         while(maxcount>=0&&count>=0&&pointer>=0)   // was just "pointer", which is
        {                                            // false at index 0 -- the oldest
            s_h.push(cmd_history[pointer]);           // command could never be shown
            count--;pointer--;
        }

    }
    while(!s_h.empty())
    {
        cout<<s_h.top()<<endl;
        s_h.pop();
    }
}