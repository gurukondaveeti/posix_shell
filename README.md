# My Custom POSIX Shell

**Course:** Advanced Operating Systems
**Author:** GURUDEEPAK KONDAVEETI
**Roll Number:** 2025201038

##  Overview

This project is a custom interactive shell implemented entirely in C++ for Linux-based systems. It emulates many of the core functionalities of standard shells like `bash`, providing a robust command-line interface. The shell supports managing foreground and background processes, full I/O redirection, a suite of custom-built commands, and advanced interactive features like persistent command history.

---

##  Features Implemented

This shell was built with a modular design, separating parsing, execution, and built-in command logic.

### Core Shell Functionality
* **Dynamic Prompt**: A dynamic prompt is displayed in the familiar `username@system_name:current_directory>` format. It updates automatically when the directory changes, and the home directory is represented by `~`.
* **Command Parsing**: The shell correctly parses complex user input, handling multiple commands on one line separated by semicolons (`;`) and ignoring extraneous spaces or tabs. Semicolon splitting uses `strtok` on the raw line (see `split_semicolons` in `tokanize.cpp`), the same way `tokenize()` already split on whitespace.
* **GNU Readline Integration**: To provide a professional user experience, the shell is built using the GNU Readline library. This enables advanced line editing (e.g., using arrow keys to move the cursor) and is the foundation for the history feature. Readline's default completer already handles file/directory names for free; a custom `rl_attempted_completion_function` (`command_generator`/`command_completion` in `posix.cpp`) additionally completes **command names** -- builtins plus everything executable in `$PATH` -- when TAB is pressed on the first word of the line.

* **Pipelines**: Any number of commands can be chained with `|` (not just two), e.g. `cat file.txt | sort | head -3`. Each stage can carry its own `<`/`>`/`>>` redirection, so redirection combined with a pipeline works too (`ls | grep ".txt" > out.txt`). Builtins used as one stage of a pipeline (e.g. `history | grep ls`) are called directly in that stage's forked process rather than exec'd.

### Process Management
The shell can create and manage child processes to execute commands.
* **Foreground Processes**: External commands (like `sort`, `gcc`, or any user executable) are run in the foreground. The shell waits for the command to complete before showing a new prompt. This is achieved using the `fork()`, `execvp()`, and `waitpid()` system calls.
* **Background Processes**: By appending an `&` to a command, it can be run in the background. The shell prints the process ID (PID) of the new background job and immediately returns to the prompt, allowing for concurrent operations.

### I/O Redirection
Full I/O redirection is supported for both built-in and external commands, using `open()` and `dup2()` to manipulate file descriptors.
* **Input (`<`)**: A command's standard input can be taken from a file.
* **Output (`>`)**: A command's standard output can be written to a file, overwriting it if it exists.
* **Append (`>>`)**: A command's standard output can be appended to the end of a a file.

### Built-in Commands
Several standard commands were implemented from scratch without using `execvp`, as per the project requirements.

* **`cd`**: Changes the current working directory. It fully supports:
    * `cd .` (stays in the current directory)
    * `cd ..` (moves to the parent directory)
    * `cd ~` (moves to the home directory)
    * `cd -` (moves to the previously visited directory)
    * **Example Output**: For `cd -`, the shell prints the path of the directory it is switching to.
        ```
        /home/user/previous_directory
        ```

* **`pwd`**: Prints the absolute path of the current working directory using the `getcwd()` system call.
    * **Example Output**:
        ```
        /home/user/projects/os_assignment
        ```

* **`echo`**: Prints its arguments back to the standard output.
    * **Example Output**:
        ```
        user@system:~> echo hello world
        hello world
        ```

* **`ls`**: A custom implementation of the `ls` command that lists directory contents. It supports the `-l` (long format) and `-a` (show all, including hidden) flags.
    * **Example Output for `ls -l`**:
        ```
        drwxr-xr-x  4 user group 4096 Sep 04 22:30 src
        -rw-r--r--  1 user group  832 Sep 03 18:00 makefile
        -rwxr-xr-x  1 user group 24576 Sep 04 22:31 my_shell
        ```

* **`pinfo`**: Displays detailed information about a process (its own or a given PID) by reading directly from the Linux `/proc` filesystem.
    * **Example Output for `pinfo <pid>`**:
        ```
        pid -- 12345
        Process Status -- {S+}
        memory -- 234567 {Virtual Memory}
        Executable Path -- /usr/bin/gedit
        ```

* **`search`**: A utility to recursively search for a file or directory.
    * **Example Output**:
    ```
        user@system:~> search my_file.cpp
        True
    ``` 

* **`history`**: A persistent command history feature.
    * Saves the last 20 commands.
    * `history` displays the last 10 commands by default; `history <n>` displays the last `n`.
    * History is saved to a file in the home directory on exit and reloaded on startup.
    * Provides Up and Down arrow key navigation of commands through Readline.
    * **Example Output for `history`**:
        ```
          1    ls -l
          2    cd src
          3    sort lines.txt
          4    make
          5    ./my_shell
          6    pinfo 1234
          7    history
        ```

### Signal Handling
The shell correctly handles common keyboard signals for job control.
* **`Ctrl+C` (`SIGINT`)**: Interrupts the running foreground process without killing the shell itself.
* **`Ctrl+Z` (`SIGTSTP`)**: Stops the running foreground process.
* **`Ctrl+D` (EOF)**: Logs out of and exits the shell.

---

##  How to Compile and Run

A `makefile` is included for easy compilation.

1.  **Compile the program:**
    ```bash
    make
    ```
2.  **Run the shell:**
    ```bash
    ./my_shell
    ```
3.  **Clean up build files:**
    ```bash
    make clean
    ```
---

##  File Structure

The project is organized into several files, each responsible for a specific module of the shell's functionality.

* **`makefile`**: Compiles all source files and links necessary libraries.
* **`header.h`**: The main header file containing function prototypes and standard library includes.
* **`posix.cpp`**: Contains the `main()` function, the primary shell loop, and command dispatching logic,and built-in commands like `cd`.
* **`tokanize.cpp`**: Implements the sophisticated parser for user input.
* **`fg_bg.cpp`**: Contains the logic for launching and managing foreground and background processes.
* **`I_O.cpp`**: Implements the functions for handling I/O redirection.
* **`ls.cpp`**: Contains the custom implementation of the `ls` command.
* **`search.cpp`**: Contains the implementation for the `search` command.
* **`pipes.cpp`**: Splits a pipeline into stages, wires them together with `pipe()`/`dup2()`, and runs each one (builtin or `execvp`).
* **`history.cpp`**: Contains the history storage functions. History is kept at `$HOME/.history_file` so it survives regardless of which directory the shell is launched from.
* **`ctr_c_z.cpp`**: Contains the `SIGINT`/`SIGTSTP` handler (`ctrl_c_z`) and the `string vector -> char* vector` helper (`str_to_cptr`) used when exec'ing.
* **`pwd_pinfo.cpp`**: Contains the pwd and pinfo functions.