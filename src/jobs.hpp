#ifndef JOBS_HPP
#define JOBS_HPP

#include <vector>
#include <string>

struct Job {
    int jobId;
    pid_t pid;
    std::vector<std::string> command;
    std::string status; //"Running", "Stopped", "Done"
};

class Jobs {
private:
    std::vector<Job> jobs;
    int nextJobId;
public:
    Jobs();
    void addJob(pid_t pid, const std::vector<std::string>& command);
    void updateJobStatus(pid_t pid, const std::string& newStatus);
    void listJobs();
    int getLastJobId();
    bool bringToForeground(int jobId);
    bool continueBackground(int jobId);
};

#endif 
