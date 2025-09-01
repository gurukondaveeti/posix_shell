#include "header.h"

#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h> 
#include <grp.h> 
#include <ctime> 
vector<string>flag_dir(bool &a,bool&l,vector<string>args)
{
    // Parse flags/options
    vector<string>result;
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i][0] == '-') {
            for (char flag : args[i].substr(1)) {
                switch (flag) {
                    case 'a': a = true; break;
                    case 'l': l = true; break;
                    default:
                        cerr << "ls: invalid option -- '" << flag << "'\n";
                        return result;
                }
            }
        }
         else {
            result.push_back(args[i]);
        }
       
    }

     return result;
}


void print_long_format(const string& full_path, const string& name) {
    struct stat file_stat;
    // Use lstat instead of stat to handle symbolic links correctly
    if (lstat(full_path.c_str(), &file_stat) == -1) {
        perror(("stat failed for " + name).c_str());
        return;
    }

    
    // File type (first character)
if (S_ISDIR(file_stat.st_mode)) cout << "d";
else if (S_ISLNK(file_stat.st_mode)) cout << "l";
else cout << "-";

// 2. User permissions
cout << ((file_stat.st_mode & S_IRUSR) ? "r" : "-");
cout << ((file_stat.st_mode & S_IWUSR) ? "w" : "-");
cout << ((file_stat.st_mode & S_IXUSR) ? "x" : "-");

// 3. Group permissions
cout << ((file_stat.st_mode & S_IRGRP) ? "r" : "-");
cout << ((file_stat.st_mode & S_IWGRP) ? "w" : "-");
cout << ((file_stat.st_mode & S_IXGRP) ? "x" : "-");

// 4. Others permissions
cout << ((file_stat.st_mode & S_IROTH) ? "r" : "-");
cout << ((file_stat.st_mode & S_IWOTH) ? "w" : "-");
cout << ((file_stat.st_mode & S_IXOTH) ? "x" : "-");

cout << " ";  // space after permissions

    // 3. Owner and Group Name
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    cout << pw->pw_name << " " << gr->gr_name << " ";

    // 4. Size
    cout << file_stat.st_size << " ";

    // 5. Modification Time
    char time_buf[80];
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&file_stat.st_mtime));
    cout << time_buf << " ";

    // 6. File Name
    //cout << name << endl;
}





void list_directory(const string& path, bool show_all, bool long_format,const string& name) {
   
    string dir_path = path;
    if (dir_path == "~") {
        dir_path = home_dir; 
    }

    DIR* dir = opendir(dir_path.c_str());
    if (dir == NULL) {
        perror(("ls: cannot access " + path).c_str());
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string entry_name = entry->d_name;

        // The `-a` flag logic: skip hidden files if `show_all` is false
        if (!show_all && entry_name[0] == '.') {
            continue;
        }
        if(long_format)
        {
            print_long_format(dir_path, name);
        }
        // For now, we just print the name. We'll add -l logic next.
        cout << entry_name << endl;
    }

    closedir(dir);
}

void ls(const vector<string>& args) {
    bool a_flag = false;
    bool l_flag = false;
    vector<string> dir_path;

  dir_path=flag_dir(a_flag,l_flag,args);

  //now check for directorires
    if (dir_path.empty()) {
        dir_path.push_back(".");
    }

    // 2. Process each target
    for (size_t i = 0; i < dir_path.size(); i++) {
        string curr_dir = dir_path[i];
        if (curr_dir == "~") 
        {
            curr_dir = home_dir;
        }

        // If there are multiple dir_path, print the name of the directory
        if (dir_path.size() > 1) {
            std::cout << dir_path[i] << ":" << std::endl;
        }

        struct stat file_stat;
        if (lstat(curr_dir.c_str(), &file_stat) == 0) {
            if (S_ISDIR(file_stat.st_mode)) {
                list_directory(curr_dir, a_flag, l_flag, dir_path[i]);
            } 
            else { // If the curr_dir is a file, not a directory
                if (l_flag) {
                    print_long_format(curr_dir, dir_path[i]);
                    cout << dir_path[i] << endl;
                } else {
                    std::cout << dir_path[i] << std::endl;
                }
            }
        } else {
             perror(("ls: cannot access '" + dir_path[i] + "'").c_str());
        }
        
        // Add a newline between listings for multiple dir_path
        if (i < dir_path.size() - 1) {
            std::cout << std::endl;
        }
    }
}

    


