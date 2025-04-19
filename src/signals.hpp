#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <csignal>
#include <unistd.h>

extern pid_t foreground_pid;
extern int lastExitStatus;

void handleSigint(int sig, siginfo_t *info, void *context);

void setupSigintHandler();

void handleSigtstp(int sig, siginfo_t *info, void *context);

void setupSigtstpHandler();

void setForegroundPid(int pid);

void setLastExitStatus(int status);

#endif
