#include <cstring>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <thread>

#include "gaux.h"
#include "gsignal.h"
#include "gtrace.h"

using namespace std;

struct Param {
    int port_{1818};

	bool parse(int argc, char* argv[]) {
        if (argc > 2) {
			usage();
			return false;
		}

        if (argc == 2)
            port_ = atoi(argv[1]);

        return true;
	}

    static void usage() {
        cerr << "ssdemon version " << GAux::getVersion() << endl;
		cerr << endl;
        cerr << "syntax: sudemon [<port>]\n";
        cerr << "sample: sudemon 1818\n";
	}
} param;

int sd{0};

void signalHandler(int signo) {
	const char* signal = GSignal::getString(signo);
	char* msg = strsignal(signo);
	GTRACE("signo=%s(%d) %s", signal, signo, msg);
	if (signo == SIGHUP)
		return;
	if (signo == SIGSEGV)
		exit(-1);
    GTRACE("bef close(sd %d)", sd);
    ::close(sd);
    GTRACE("bef close(sd %d)", sd);
}

void recvThread(int sd) {
    printf("connected\n");
    fflush(stdout);
    static const int BUFSIZE = 65536;
    char cmd[BUFSIZE];
    while (true) {
        ssize_t res = ::recv(sd, cmd, BUFSIZE - 1, 0);
        if (res == 0 || res == -1) {
            GTRACE("recv return %zd", res);
            break;
        }
        cmd[res] = '\0';
        GTRACE("%s", cmd); // gilgil temp 2026.02.28

        FILE *fp;
        char path[1035];

        fp = popen("ls -l", "r");
        if (fp == nullptr) {
            GTRACE("fail to popen(%s)", buf);
            exit(1);
        }

        // 결과 한 줄씩 읽어서 출력
        while (fgets(path, sizeof(path), fp) != NULL) {
            printf("받은 데이터: %s", path);
        }

        // 파이프 닫기
        pclose(fp);

        fflush(stdout);
        if (param.echo) {
            res = ::send(sd, buf, res, 0);
            if (res == 0 || res == -1) {
                fprintf(stderr, "send return %zd", res);
                myerror(" ");
                break;
            }
        }
    }
    printf("disconnected\n");
    fflush(stdout);
    ::close(sd);
}

int main(int argc, char* argv[]) {
    if (!param.parse(argc, argv)) {
        Param::usage();
        return -1;
    }

    gtrace_default("127.0.0.1", 8908, false, "sudemon.log");

	char wd[BUFSIZ];
	memset(wd, 0, BUFSIZ);
	if (getcwd(wd, BUFSIZ) == nullptr)
		GTRACE("getcwd return null");
	int res = chdir(GAux::getDir(argv[0]).data());
	if (res != 0)
		GTRACE("chdir return %d", res);
    GTRACE("sudemon %s started login=%s dir=%s %s %s", GAux::getVersion(), getlogin(), wd, __DATE__, __TIME__);

	GSignal::instance().setupAll(signalHandler);

    //
    // socket
    //
    int sd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        myerror("socket");
        return -1;
    }

#ifdef __linux__
    //
    // setsockopt
    //
    {
        int optval = 1;
        int res = ::setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        if (res == -1) {
            myerror("setsockopt");
            return -1;
        }
    }
#endif // __linux

    //
    // bind
    //
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(param.port_);

        ssize_t res = ::bind(sd, (struct sockaddr *)&addr, sizeof(addr));
        if (res == -1) {
            myerror("bind");
            return -1;
        }
    }

    //
    // listen
    //
    {
        int res = listen(sd, 5);
        if (res == -1) {
            myerror("listen");
            return -1;
        }
    }

    while (true) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int newsd = ::accept(sd, (struct sockaddr *)&addr, &len);
        if (newsd == -1) {
            myerror("accept");
            break;
        }

        std::thread* t = new std::thread(recvThread, newsd);
        t->detach();
    }
    ::close(sd);

    GTRACE("sudemon terminated successfully");
}
