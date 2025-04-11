/* ICCS227: Project 1: icsh
 * Name:
 * StudentID:
 */

 #include "common.hpp"
 #include "parser.hpp"
 #include "executor.hpp"
 #include "history.hpp"
 #include "jobs.hpp"

 
 
 int main() {
     char buffer[MAX_CMD_BUFFER];
     while (1) {
         printf("icsh $ ");
         fgets(buffer, 255, stdin);
         printf("you said: %s\n", buffer);
     }
 }