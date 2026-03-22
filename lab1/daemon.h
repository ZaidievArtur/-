#pragma once

#include <string>
#include <atomic>

class Daemon {
private:
    std::string configPath;
    std::string dir;
    std::string pidFile;
    int interval;

    std::atomic<bool> running;
    std::atomic<bool> reloadConfig;

    Daemon();

public:
    static Daemon& getInstance();

    void setConfigPath(const std::string& path);
    void loadConfig();
    void daemonize();
    void handleSignals();
    void run();

    void stop();
    void requestReload();
};
