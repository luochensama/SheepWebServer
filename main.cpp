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
    int threadNum = 4;
    int port = 12335;
    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}

