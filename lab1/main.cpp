#include "daemon.h"

#include <unistd.h>
#include <limits.h>

int main() {
    Daemon& d = Daemon::getInstance();

    char path[PATH_MAX];
    realpath("config.txt", path);

    d.setConfigPath(path);
    d.loadConfig();
    d.daemonize();
    d.handleSignals();
    d.run();

    return 0;
}
