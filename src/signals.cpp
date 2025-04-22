#include "signals.hpp"
#include <signal.h>
#include <unistd.h>
#include <iostream>


pid_t foreground_pid = -1;
int lastExitStatus = 0;
bool isSignal = false;

void handleSigint(int sig, siginfo_t *info, void *context) {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT);
    } else {
        isSignal = true;
        std::cout << "No foreground process running. Can't Use Ctrl + C" << std::endl;
    }
}

void setupSigintHandler() {
    struct sigaction sa;
    sa.sa_sigaction = handleSigint;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &sa, nullptr);
}


void handleSigtstp(int sig, siginfo_t *info, void *context) {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGTSTP);
    } else {
        isSignal = true;
        std::cout << "No foreground process running.Can't Use Ctrl + Z" << std::endl;
    }
}

void setupSigtstpHandler() {
    struct sigaction sa;
    sa.sa_sigaction = handleSigtstp;
    sigfillset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGTSTP, &sa, nullptr);
}

void setForegroundPid(int pid) {
    foreground_pid = pid;
}

void setLastExitStatus(int status) {
    lastExitStatus = status;
}
