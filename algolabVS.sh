#!/bin/bash
# Created by nicbauma, Autumn Semester 2018, 
# modified by bittero, Autumn Semester 2019
VERSION="0.1"

# Edit command (executed in current working directory)
IDE_COMMAND='qtcreator CMakeLists.txt &>/dev/null &disown'

# Enable cmake generators for ide project files (not tested)
USE_CODEBLOCKS=false
USE_ECLIPSE=false
USE_KATE=false

# Usage
DOC=$(cat <<'EOF'
///////////////////////////////////
///       Algolab VS usage      ///
///////////////////////////////////

## 1 Setup (Only do this once.)

### 1.1 Install C++ Extension For VS Code
1. Download the file `ms-vscode.cpptools-0.21.0.vsix` from the `algolab/tools/` directory on the judge.
2. cd to the directory where you saved `ms-vscode.cpptools-0.21.0.vsix`.
3. Run `code --install-extension ms-vscode.cpptools-0.21.0.vsix`

You should now be able to use c++ intellisense, goto definition etc. 

REMARK: Be aware that it takes a while for parsing all the c++ libraries, so autocomplete won't be available immediately.
On the ETH lab computers it takes up 8-10 minutes, so be patient (you can keep coding meanwhile).
 Also, check in the bottom right corner, when hovering over the little DB icon, whether it states 'Parsing Paused'. If yes, click the symbol and resume parsing.

### 1.2 Install algolabVS command
1. Download the this file and place it wherever you want.
2. Cd to the directory where you placed this file.
3. Run `chmod +x ./algolabVS.sh` to make the script executable.
4. Run `./algolabVS.sh install`

You can now use the algolabVS command in all directories.

## 2 Initialize a new project (Do this once for every task.)

1. cd to the folder where you want to init your project.
2. Run `algolabVS init`
3. Run `code .` to open VS code in the current directory.


Now VS code will be configured to run/debug/test within VS code. 

### 2.1 How to run tasks
1. Open the VS code command prompt (`Ctrl + shift + P` on windows/linux, probabily `Cmd + shift + P` on OSX?).
2. Type `run task` and look for the command `Tasks: Run Task`.
3. Select the command and see a list of three tasks to run which are explained below:

- Run: Compiles in release mode and runs the program in the inegrated terminal (Runs 'algolab run' under the hood.)
- Debug: Compiles in debug mode but DOES NOT run the program. See section 'How to use the debugger' below. (Runs a slightly modified version of 'algolab debug' under the hood.)
- Test: Compiles in release mode, runs the program with all testcases and prints the diff to the integrated terminal. (Runs 'algolab test' under the hood.)

### 2.2 How to use the debugger
To use the debugger, do not run the debug task above directly. Simply go to the debug section of VS code and press the play button in the top left corner. This will run the above debug task under the hood. Alternatively one can also press `F5` which is the default keybinding in VS code.
You should now be able to use the GDB debugger within vs code!
As it is annoying to input the data manually while debugging, the input will be redirected from a file called 'debug_test_data.txt' inside the root directory. Simply write down your input there before running the debugger.


///////////////////////////////////
///  Vanilla algolab.sh usage   ///
///////////////////////////////////

As algolabVS is simply a wrapper for the original algolab.sh script, the remaining options are still available. 
Don't forget that the command name is `algolabVS` (not `algolab`).

install: "Install" the script into user home (including autocomplete for commands after shell restart).
After installation, script can be invoked in any directory using the alias 'algolabVS'.

init: Setup project (creates CMakeLists.txt, algorithm.cpp, build directories (both debug and release), vs code config files, git setup and runs cmake)

clean: Remove build directory and CMakeLists.txt

edit: Execute IDE_COMMAND

debug: Compile in debug mode and open the executable file with gdb (in current working dir)

memcheck: Compile in debug mode (if modified) and run with valgrind

compile: Compiles in release mode

run: Compiles in release mode (if modified) and launches the executable (in current working dir)

test: Compiles in relase mode (if modified) and runs all test cases (compares all .in files with .out files). If a test
fails, mismatching output file lines are highlighted in red (lines are truncated to fit terminal window)

version: Show version number
EOF
)

# Colors
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
NC=$(tput sgr0)

# Used for test output truncation
minlen=7
maxlen=$(($(tput cols)/2-5))
cutlen="-$((maxlen-3))"

if [ -f ".algolabVS" ]; then
    pname=$(cat ".algolabVS")
fi
pname=${pname:-algo}
pname=${2:-${pname}}

# File templates

CMAKE_FILE=$(cat <<'EOF'
cmake_minimum_required(VERSION 2.8)
project(pname)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

find_package(CGAL REQUIRED COMPONENTS Core)
find_package(Boost REQUIRED)
include_directories(${CGAL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
include( ${CGAL_USE_FILE} )

add_executable(${PROJECT_NAME} "src/algorithm.cpp")
target_link_libraries(${PROJECT_NAME} ${CGAL_LIBRARIES} ${Boost_LIBRARIES} ${MPFR_LIBRARIES} ${GMP_LIBRARIES} ${THREAD_LIBRARIES})
EOF
)
CMAKE_FILE="${CMAKE_FILE/pname/${pname}}"

CXX_TEMPLATE=$(cat <<'EOF'
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	// TODO:
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
EOF
)

COMP_FILE=$(cat <<EOF
#/usr/bin/env bash
alias algolabVS='~/.algolabVS.sh'
complete -W "clean init edit debug memcheck run compile test version" algolabVS
EOF
)

#-------------------------------------------------------------------------------
# Config files for VS code
#-------------------------------------------------------------------------------

algolabVS_script="~/Documents/ethz/algolab/algolabVS.sh"
TASKS=$(cat <<EOF
{
  // See https://go.microsoft.com/fwlink/?LinkId=733558
  // for the documentation about the tasks.json format
  "version": "2.0.0",
  "tasks": [
    {
      "label": "run",
      "type": "shell",
      "command": "${algolabVS_script}",
	  "args": [
        "run"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    },
    {
      "label": "test",
      "type": "shell",
      "command": "${algolabVS_script}",
	  "args": [
        "test"
      ],
      "group": "test"
    },
    {
      "label": "debug",
      "type": "shell",
      "command": "${algolabVS_script}",
	  "args": [
        "debug"
      ],
      "group": "none"
    },
  ]
}
EOF
)


LAUNCH=$(cat <<'EOF'
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "(gdb) Launch",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/debug/algo",
      "args": [
        "<",
        "${workspaceFolder}/debug_test_data.txt"
      ],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "preLaunchTask": "debug",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ]
    }
  ]
}
EOF
)

# Helper function
function try_compile() {
    local ok
    local coutput
    coutput=$(make 2>&1)
    ok=$?
    if [ "$ok" != 0 ]; then
        echo "Compiler error!"
        echo ""
        echo "$coutput"
        exit 1
    fi
}

case "$1" in
    version)
        echo "${VERSION}"
        ;;
    install)
        cp "$0" ~/.algolabVS.sh
        if [ ! -f ~/.bashrc ] || ! grep -q 'source ~/.algolabVS-bashrc.bash' ~/.bashrc; then
            echo 'source ~/.algolabVS-bashrc.bash' >> ~/.bashrc
        fi
        echo "$COMP_FILE" > ~/.algolabVS-bashrc.bash
        exit 0
        ;;
    clean)
    	if [ -f .algolabVS ]; then
            rm -r build
            rm CMakeLists.txt
            echo "algo" > .algolabVS
        else
            echo "Not an algolabVS project directory."
    	fi
        ;;
    init)
        printf "Creating CMake project...\t"
        echo "$pname" > .algolabVS
        mkdir -p src
        if [ ! -f src/algorithm.cpp ]; then
            echo "$CXX_TEMPLATE" > src/algorithm.cpp
        fi
        echo "$CMAKE_FILE" > CMakeLists.txt
        mkdir -p build
        mkdir -p build/debug
        mkdir -p build/release

        #----------------------------------------------------------------------
        # Config files for VS code
        #----------------------------------------------------------------------
        destination=$PWD
        vscode_directory="${destination}/.vscode"
        mkdir "$vscode_directory"

        debug_test_data_filename="${destination}/debug_test_data.txt"
        launch_filename="${vscode_directory}/launch.json"
        tasks_filename="${vscode_directory}/tasks.json"

        # Create files 
        if [ ! -f "${launch_filename}" ]; then
            echo "$LAUNCH" > "$launch_filename"
        fi

        if [ ! -f "${tasks_filename}" ]; then
            echo "$TASKS" > "$tasks_filename"
        fi
        if [ ! -f "${debug_test_data_filename}" ]; then
            echo "0" > "${debug_test_data_filename}"
        fi
        echo ""
    	echo "Created VS code config files"
    	echo ""
        
        cd build/debug
        cmake -DCMAKE_BUILD_TYPE=Debug -DCGAL_DONT_OVERRIDE_CMAKE_FLAGS=True ../..
        echo ""
    	echo "Created Debug Makefile"
    	echo ""
        cd ../release
        cmake -DCMAKE_BUILD_TYPE=release ../..
        echo ""
    	echo "Created Release Makefile"
    	echo ""
        cd ../
        mkdir -p project_files
        cd project_files
        if [ "$USE_CODEBLOCKS" == true ]; then
        	cmake ../.. -G"CodeBlocks - Unix Makefiles"
        	echo ""
        	echo "Created Code::Blocks project"
        	echo ""
        fi
        if [ "$USE_ECLIPSE" == true ]; then
	        cmake ../.. -G"Eclipse CDT4 - Unix Makefiles"
	        echo ""
        	echo "Created Eclipse project"
        	echo ""
        fi
        if [ "$USE_KATE" == true ]; then
	        cmake ../.. -G"Kate - Unix Makefiles"
	        echo ""
        	echo "Created Kate project"
        	echo ""
        fi
        cd ../..
        echo ""
        echo "Finished creating project files."
        echo ""
        ;;
    edit)
        eval "$IDE_COMMAND"
        ;;
    debug)
        cd build/debug
        try_compile
        cd ../..
        # gdb --exec="build/debug/${pname}"
        ;;
    memcheck)
        cd build/debug
        try_compile
        cd ../..
        valgrind "build/debug/${pname}"
        ;;
    compile)
        cd build/release
        make
        ;;
    run)
        cd build/release
        try_compile
        cd ../..
        echo "Running ${pname}:"
        echo ""
        "./build/release/${pname}"
        ;;
    test)
        cd build/release
        try_compile
        cd ../..
        echo "Running all tests for ${pname}:"
        echo ""
        find . -name '*.in' | sort | while read line; do
            tname=${line::-3}
            printf "Test '${tname:2}': "
            result=$("./build/release/${pname}" < "$tname.in")
            ok=$?
            if [ "$ok" != 0 ]
            then
                echo "Runtime error!"
            else
                expected=$(cat "$tname.out")
                
                if [ "$result" == "$expected" ]
                then
                    printf "${GREEN}PASSED${NC}\n"
                else
                    # Truncate lines if longer than half terminal
                    len=$(echo "$expected" | awk '(NR==1||length>longest){longest=length} END {print longest}')
                    if [ "$len" -gt "$maxlen" ]; then 
                        len="$maxlen" 
                    fi
                    if [ "$len" -lt "$minlen" ]; then
                        len=$minlen
                    fi
                    len=$((len+3))
                    dashes=$(printf "%-$((len*2))s" "-")
                
                    printf "${RED}FAILED${NC}\n"
                    
                    echo "${dashes// /-}"
                    printf "%-${len}s %-${len}s\n" "Expected" "Was"
                    echo "${dashes// /-}"
                    while read -u3 e; read -u4 w;
                    do
                        equal=0; [ "$e" == "$w" ] && equal=1
                        
                        if [ "${#e}" -gt "$maxlen" ]; then
                            e="$(echo "$e" | cut -c $cutlen)..."
                        fi
                        if [ "${#w}" -gt "$maxlen" ]; then
                            w="$(echo "$w" | cut -c $cutlen)..."
                        fi
                        
                        if [ "$equal" == 0 ]; then
                            printf "${RED}%-${len}s %-${len}s${NC}\n" "$e" "$w"
                        else
                            printf "%-${len}s %-${len}s\n" "$e" "$w"
                        fi
                        
                    done 3< <(echo "$expected") 4< <(echo "$result")
                    echo "${dashes// /-}"
                    echo ""
                fi
            fi
        done
        ;;
    *)
        echo "Usage: algolabVS (init|clean|edit|debug|memcheck|compile|run|test|version)"
        echo ""
        echo "$DOC"
        echo ""
esac
