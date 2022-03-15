#include <bits/stdc++.h>
#include <unistd.h>
#include "base/noncopyable.h"
#include "base/Logging.h"
#include "EventLoop.h"
#include "sys/timerfd.h"
#include "HttpContext.h"
#include "Server.h"

using namespace std;

int main(int argc, char *argv[]) {
    int threadNum = 5;
    int port = 80;

    int opt;
    const char *str = "t:l:p:";
    while ((opt = getopt(argc, argv, str)) != -1) {
        switch (opt) {
            case 't': {
                threadNum = atoi(optarg);
                break;
            }
            case 'p': {
                port = atoi(optarg);
                break;
            }
            default:
                break;
        }
    }

    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}