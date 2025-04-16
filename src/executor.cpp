#include "executor.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <iostream>

int execute_external(const std::vector<std::string> &tokens) {

    if(tokens.empty()) {
        return 0;
    }

   std::vector<char*> args;
   //using const is faster than using std::string in terms of converting to char* (from benchmarking)
   for (const auto &token: tokens) {
    //exec() functions expect char*, not std::string 
    // --> use .c_str() to convert fron const std::string to const char*
    // --> use const_cast to convert from const char* to char*
    args.push_back(const_cast<char*>(token.c_str()));
   }

   // exec() functions expects a null pointer at the end of array input
   args.push_back(nullptr);

   pid_t pid = fork();

   if (pid < 0) {
        std::cerr << "Fork failed." << std::endl;
        return -1;
   } else if (pid == 0) {
        int val = execvp(args[0], args.data());
        if (val == -1) {
            std::cerr << "Failed to execute command: " 
                    << strerror(errno) << std::endl;
            exit(1); //exit(0) for success, exit(1) for failure
        }
   } else {
        int status;
        wait(&status);
        return status;
   }

   return 0;

}