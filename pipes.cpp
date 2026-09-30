#include "header.h"

int if_pipe(const vector<string>& tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if ( tokens[i] == "|") {
            return i;
        }
    }
    return -1;

}

// Splits args into one vector<string> per pipeline stage, wherever "|"
// shows up -- "cat a | sort | head -3" becomes {{"cat","a"}, {"sort"},
// {"head","-3"}}. This is what lets pipes() below handle any number of
// pipes instead of exactly one.
vector<vector<string>> split_pipe_stages(const vector<string>& args)
{
    vector<vector<string>> stages;
    vector<string> current;
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "|") {
            stages.push_back(current);
            current.clear();
        } else {
            current.push_back(args[i]);
        }
    }
    stages.push_back(current);
    return stages;
}

// Pulls "<"/">"/">>" (and the filename after each) out of one stage's
// tokens and applies them with open()+dup2(), same approach as I_O.cpp.
// This is what makes requirement 9 (redirection inside a pipeline, e.g.
// "cat < in.txt | wc -l > out.txt") work per-stage. Returns the remaining
// real command tokens.
vector<string> apply_stage_redirection(const vector<string>& stage)
{
    vector<string> cmd;
    string input_file, output_file;
    bool append = false;

    for (size_t i = 0; i < stage.size(); ++i) {
        if (stage[i] == "<") {
            if (i+1 < stage.size()) { input_file = stage[i+1]; i++; }
        }
        else if (stage[i] == ">") {
            if (i+1 < stage.size()) { output_file = stage[i+1]; i++; }
        }
        else if (stage[i] == ">>") {
            if (i+1 < stage.size()) { output_file = stage[i+1]; i++; append = true; }
        }
        else {
            cmd.push_back(stage[i]);
        }
    }

    if (!input_file.empty()) {
        int fd = open(input_file.c_str(), O_RDONLY);
        if (fd < 0) { perror("while opening input"); exit(EXIT_FAILURE); }
        dup2(fd, 0);
        close(fd);
    }
    if (!output_file.empty()) {
        int fd;
        if (!append) fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else         fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) { perror("while opening output"); exit(EXIT_FAILURE); }
        dup2(fd, 1);
        close(fd);
    }

    return cmd;
}

// Runs one pipeline stage's actual command. Only ever called inside a
// forked child (never returns), so builtins get called directly here --
// this is what makes something like "history | grep ls" work, matching the
// same "run the builtin in the child instead of execvp" trick I_O.cpp
// already uses for redirected builtins.
void run_stage(vector<string>& cmd)
{
    if (cmd.empty()) exit(EXIT_SUCCESS);

    if (cmd[0] == "echo") { echo(cmd); }
    else if (cmd[0] == "pwd") { pwd(); }
    else if (cmd[0] == "ls") { ls(cmd); }
    else if (cmd[0] == "pinfo") { pinfo(cmd); }
    else if (cmd[0] == "history") { print_history(cmd); }
    else if (cmd[0] == "search") {
        if (cmd.size() != 2) cout << "arguments mismatch" << endl;
        else if (search(".", cmd[1])) cout << "True" << endl;
        else cout << "False" << endl;
    }
    else {
        vector<char*> c_args = str_to_cptr(cmd);
        execvp(c_args[0], c_args.data());
        perror(c_args[0]);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

// pos is kept in the signature so the one call site in posix.cpp doesn't
// need to change, but it's no longer used for the split -- we re-scan args
// for every "|" via split_pipe_stages() instead of just the first one.
//
// Old version of this function forked exactly once and had its PARENT
// branch call execvp() directly -- since pipes() is called straight from
// the main shell process (no enclosing fork in posix.cpp), that replaced
// the actual interactive shell with the left-hand command. This version
// forks once PER STAGE from the still-alive shell process and waits for
// all of them, so the shell survives every pipeline, and any number of
// "|" works, not just one.
void pipes(const vector<string> &args, int pos)
{
    (void)pos;

    vector<vector<string>> stages = split_pipe_stages(args);
    size_t n = stages.size();

    vector<pid_t> pids;
    int prev_read_end = -1;

    for (size_t i = 0; i < n; ++i) {
        int fd[2] = {-1, -1};
        if (i + 1 < n) pipe(fd);

        pid_t pid = fork();
        if (pid < 0) { perror("fork failed"); return; }

        if (pid == 0) {
            // child: wire this stage's stdin/stdout to its neighbours
            if (prev_read_end != -1) { dup2(prev_read_end, 0); close(prev_read_end); }
            if (i + 1 < n) { dup2(fd[1], 1); close(fd[1]); close(fd[0]); }

            vector<string> cmd = apply_stage_redirection(stages[i]);
            run_stage(cmd);   // never returns
        }

        // parent: close whatever this stage no longer needs, remember the
        // read end the NEXT stage will need as its stdin
        if (prev_read_end != -1) close(prev_read_end);
        if (i + 1 < n) { close(fd[1]); prev_read_end = fd[0]; }
        pids.push_back(pid);
    }

    fore_ground_pid = pids.back();   // so Ctrl-C/Ctrl-Z can still reach the pipeline
    for (pid_t pid : pids) {
        int status;
        waitpid(pid, &status, 0);
    }
    fore_ground_pid = 0;
}
