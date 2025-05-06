#include "jobs.hpp"
#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "executor.hpp"
#include "signals.hpp"


int nextJobId = 1;

Jobs::Jobs() : nextJobId(1) { }


// void Jobs::addJob(pid_t pid, const std::vector<std::string>& command, bool isBackgroundRunning) {
//     Job job;
//     job.jobId = nextJobId++;
//     job.pid = pid;
//     job.command = command;
//     job.backgroundRunning = isBackgroundRunning;
//     if(isBuiltIn) {
//         job.status = "Done";
//     } else {
//         job.status = "Running";
//     }
    
//     jobs.push_back(job);
// }

// void Jobs::addJob(const std::vector<std::string>& command, bool isBackgroundRunning) {
//     Job job;
//     job.jobId = nextJobId++;
//     job.command = command;
//     job.backgroundRunning = isBackgroundRunning;
//     if(isBuiltIn) {
//         job.status = "Done";
//     } else {
//         job.status = "Running";
//     }
    
//     jobs.push_back(job);
// }
void Jobs::addJob(Job job, pid_t pid) {
    job.pid = pid;
    jobs.push_back(job);
}

void Jobs::updateJobStatus(pid_t pid, const std::string& newStatus) {
    for (auto &job : jobs) {
        if (job.pid == pid) {
            job.status = newStatus;

            if(newStatus == "Done" && job.backgroundRunning) {
                std::cout << pid << "   -Done" << std::endl;
            }
            
            break;
        }
    }
}

void Jobs::listJobs() {
    for (const auto &job : jobs) {
        if (job.status == "Running" || job.status == "Stopped") {
            std::cout << "[" << job.jobId << "]\t" << job.status << "\t";
            for (auto &token : job.command)
                std::cout << token << " ";
            std::cout << std::endl;
        }
        
    }
}

int Jobs::getLastJobId() {
    if (jobs.empty()) return 0;
    return jobs.back().jobId;
}


bool Jobs::continueBackground(int jobID ) {
    for (auto &job: jobs) {
        if(job.jobId == jobID && job.status != "Done") {
            job.status = "Running";
            job.backgroundRunning = true;
             if (kill(job.pid, SIGCONT) < 0) {
                std::cout << "Failed to continue the process:  " << job.pid << std::endl;
             } else {
                std::cout << " Successfully send the SIGCONT signal to the background process " << std::endl;
             }

            
            std::cout << "[" << job.jobId << "]    " << job.pid  << " is continued" << std::endl;
            return true;
        }
    }
    return false;
}

// bool Jobs::bringToForeground(int jobID) {
//     for (auto it = jobs.begin(); it != jobs.end(); ++it) {
//         if (it->jobId == jobID) { // it->jobId == (*it).jobId
//             it->backgroundRunning = false;
//             kill(it->pid, SIGCONT);
//             int status;
//             waitpid(it->pid, &status, WUNTRACED);
//             jobs.erase(it);
//             return true;
//         }
//     }
//     return false;
// }

bool Jobs::bringToForeground(int jobID) {
    int status;
    setupSigintHandler();
    setupSigtstpHandler();
    for (auto it = jobs.begin(); it != jobs.end(); ++it) {
        if (it->jobId == jobID && it->status != "Done") { 
            pid_t pid   = it->pid;
            setForegroundPid(pid);            
            globalJobs.updateJobStatus(pid, "Running");
            if (kill(pid, SIGCONT) < 0) {
                std::cout << "Failed to bring the process into foreground:  " << pid << std::endl;
             } else {
                std::cout << " Successfully send the SIGCONT signal to the process and bring it to foreground " << std::endl;
             }
            int status;
            waitpid(pid, &status, WUNTRACED);
            
            if (WIFSTOPPED(status)) {
                setLastExitStatus(1);
                globalJobs.updateJobStatus(pid, "Stopped");
            } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
                globalJobs.updateJobStatus(pid, "Done");
                setLastExitStatus(0);
            }   
            
            return true;
        }
    }
    return false;
}

Job* Jobs::getJobById(int jobId) {
    for (auto &job : jobs) {
        if (job.jobId == jobId) {
            return &job;
        }
    }
    return nullptr;
}

Job* Jobs::getJobByPid(pid_t pid) {
    for (auto &job : jobs) {
        if (job.pid == pid) {
            return &job;
        }
    }
    return nullptr;
}


bool Jobs::isThereRunning() {
    for (auto &job: jobs) {
        if (job.status == "Running" && job.backgroundRunning) {
            return true;
        }
    }
    return false;
}
Job* Jobs::getLastJob() {
    if (jobs.empty()) return nullptr;
    return &jobs.back();             
}

std::vector<Job>* Jobs::getAllJobs() {
    return &jobs;
}