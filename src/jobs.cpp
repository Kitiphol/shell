#include "jobs.hpp"
#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

void Jobs::addJob(pid_t pid, const std::vector<std::string>& command) {
    Job job;
    job.jobId = nextJobId++;
    job.pid = pid;
    job.command = command;
    job.status = "Running";
    jobs.push_back(job);
}

void Jobs::updateJobStatus(pid_t pid, const std::string& newStatus) {
    for (auto &job : jobs) {
        if (job.pid == pid) {
            job.status = newStatus;
            break;
        }
    }
}

void Jobs::listJobs() {
    for (const auto &job : jobs) {
        std::cout << "[" << job.jobId << "]\t" << job.status << "\t";
        for (auto &token : job.command)
            std::cout << token << " ";
        std::cout << std::endl;
    }
}

int Jobs::getLastJobId() {
    if (jobs.empty()) return 0;
    return jobs.back().jobId;
}

