#include "daemon.h"

#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>
#include <filesystem>
#include <signal.h>
#include <cstring>

namespace fs = std::filesystem;

static Daemon* instance = nullptr;

Daemon::Daemon() : interval(5), running(true), reloadConfig(false) {}

Daemon& Daemon::getInstance() {
    if (!instance)
        instance = new Daemon();
    return *instance;
}

void Daemon::setConfigPath(const std::string& path) {
    configPath = path;
}

void Daemon::loadConfig() {
    std::ifstream file(configPath);
    std::string line;

    while (getline(file, line)) {
        if (line.find("dir=") == 0)
            dir = line.substr(4);
        else if (line.find("interval=") == 0)
            interval = std::stoi(line.substr(9));
        else if (line.find("pidfile=") == 0)
            pidFile = line.substr(8);
    }

    syslog(LOG_INFO, "Config loaded: dir=%s interval=%d", dir.c_str(), interval);
}

static void createPidFile(const std::string& pidFile) {
    std::ifstream in(pidFile);
    if (in) {
        int oldPid;
        in >> oldPid;
        if (kill(oldPid, 0) == 0) {
            kill(oldPid, SIGTERM);
        }
    }

    std::ofstream out(pidFile);
    out << getpid();
}

void Daemon::daemonize() {
    pid_t pid = fork();

    if (pid < 0) _exit(EXIT_FAILURE);
    if (pid > 0) _exit(EXIT_SUCCESS);

    if (setsid() < 0) _exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0) _exit(EXIT_FAILURE);
    if (pid > 0) _exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    openlog("mydaemon", LOG_PID, LOG_DAEMON);
}

static void signalHandler(int sig) {
    Daemon& d = Daemon::getInstance();

    if (sig == SIGTERM)
        d.stop();
    else if (sig == SIGHUP)
        d.requestReload();
}

void Daemon::handleSignals() {
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);
}

void Daemon::stop() {
    running = false;
}

void Daemon::requestReload() {
    reloadConfig = true;
}

void Daemon::run() {
    createPidFile(pidFile);

    while (running) {
        if (reloadConfig) {
            loadConfig();
            reloadConfig = false;
        }

        if (!fs::exists(dir)) {
            syslog(LOG_ERR, "Directory not found: %s", dir.c_str());
            sleep(interval);
            continue;
        }

        std::string protectFile = dir + "/dont.erase";

        if (!fs::exists(protectFile)) {
            for (const auto& entry : fs::directory_iterator(dir)) {
                fs::remove_all(entry.path());
            }
            syslog(LOG_INFO, "Directory cleaned");
        } else {
            syslog(LOG_INFO, "dont.erase exists, skipping");
        }

        sleep(interval);
    }

    syslog(LOG_INFO, "Daemon stopped");
    closelog();
}
