

#include "header.h"



bool is_directory(string& path) {
   
     struct stat st;

    if (stat(path.c_str(), &st) == 0) {
    
    if (S_ISDIR(st.st_mode)) {
        return 1;
    }    
    }
    return 0;
}

    

bool is_dot_dir(const string& name) {
    return name == "." || name == "..";
}


bool search(const string& root, const string& target) {
    queue<string> rem_dir;
    rem_dir.push(root);

    while (!rem_dir.empty()) {
        string current_dir = rem_dir.front();
        rem_dir.pop();

        DIR* dp = opendir(current_dir.c_str());
        if (!dp) continue; // Skip directories we can't read

        

        struct dirent* entry;
        while ((entry = readdir(dp)) != NULL) {
            string name = entry->d_name;
            if (is_dot_dir(name)) continue;

            // Checking
            if (name == target) {
                closedir(dp);
                return true;
            }

            // If  directory, add it to the queue to visit later
            string full_path = current_dir + "/" + name;
            if (is_directory(full_path)) {
                rem_dir.push(full_path);
            }
        }
        closedir(dp);
    }
    return false; // Traversed everything without finding the target
}