#include "executor.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <fcntl.h>

int executeExternalCommand(const std::vector<std::string> &tokens) {

    if(tokens.empty()) {
        return 0;
    }


    int in = -1;
    int out = -1;
    std::vector<std::string> cleanTokens;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "<") {
            if (i + 1 < tokens.size()) {
                in = open(tokens[i + 1].c_str(), O_RDONLY);
                if (in < 0) {
                    std::perror("Error opening input file");
                    return -1;
                }
                ++i; 
            }
        } else if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                out = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (out < 0) {
                    std::perror("Error opening output file");
                    return -1;
                }
                ++i; 
            }
        } else {
            cleanTokens.push_back(tokens[i]);
        }
    }




   std::vector<char*> args;
   //using const is faster than using std::string in terms of converting to char* (from benchmarking)
   for (const auto &token: cleanTokens) {
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

        if (in != -1) {
            dup2(in, 0);
            close(in);
        }

        if (out != -1) {
            dup2(out, 1);
            close(out);
        }

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