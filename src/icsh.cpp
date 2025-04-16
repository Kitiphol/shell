/* ICCS227: Project 1: icsh
 * Name:
 * StudentID:
 */

 #include "common.hpp"
 #include "parser.hpp"
 #include "executor.hpp"
 #include "history.hpp"
 #include "jobs.hpp"

 #include <fstream>
 
 
 int main(int argc, char* argv[]) {
     std::string input;
     std::istream *input_stream = &std::cin;
     std::ifstream file;
     History history;

     if (argc > 1) {
        file.open(argv[1]);
        if (!file) {
            std::cerr << "Failed to open script file " << argv[1] << std::endl;
            return 1;
        }
        input_stream = &file;
    }

    while(true) {

        if (input_stream == &std::cin) {
            std::cout << "icsh $ ";
        }

        if (!std::getline(*input_stream, input)) {
            break; 
        }

        if (input.empty()) {
            continue;
        }

        if (input == "!!") {
            std::string last = history.getLastCommand();
            if (last.empty())
                continue;
            std::cout << last << std::endl;
            input = last;
        }

        history.add(input);

        auto tokens = parse_input(input);
        if (tokens.empty()) {
            continue;
        }

        if (tokens[0] == "echo") {

            for (size_t i = 1; i < tokens.size(); ++i) {
                if (tokens[i] == "$$") {
                    std::cout << getpid() << " ";
                } else {
                    std::cout << tokens[i] << " ";
                }
            }
            std::cout << std::endl;
            continue;
            
        }

        if (tokens[0] == "exit") {
            int exit_code = 0;
            if (tokens.size() > 1) {
                try {
                    exit_code = std::stoi(tokens[1]) & 0xFF; // truncate to 8 bits
                } catch (...) {
                    exit_code = 0;
                }
            }
            std::cout << "bye" << std::endl;
            return exit_code;
        }

        // For any other command, assume it is an external command.
        int status = executeExternalCommand(tokens);



    }
    return 0;

 }