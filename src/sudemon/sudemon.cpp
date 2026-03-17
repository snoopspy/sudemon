#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <thread>

#include "gaux.h"
#include "gsignal.h"
#include "gtrace.h"

struct Param {
	int port_{9999};

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
		printf("ssdemon version %s\n",  GAux::getVersion());
		printf("Copyright (c) Gilbert Lee All rights reserved\n");
		printf("\n");
		printf("syntax: sudemon [<port>]\n");
		printf("sample: sudemon 9999\n");
	}
} param;

int _acceptSocket{0};

void signalHandler(int signo) {
	const char* signal = GSignal::getString(signo);
	char* msg = strsignal(signo);
	GTRACE("signo=%s(%d) %s", signal, signo, msg);
	if (signo == SIGHUP)
		return;
	if (signo == SIGSEGV)
		exit(-1);
	GTRACE("bef close(sd %d)", _acceptSocket);
	::close(_acceptSocket);
	GTRACE("aft close(sd %d)", _acceptSocket);
}

void process(int sd) {
	fflush(stdout);
	static const int BUFSIZE = 65536;
	char buf[BUFSIZE];
	ssize_t res = ::recv(sd, buf, BUFSIZE - 1, 0);
	if (res == 0 || res == -1) {
		GTRACE("recv return %zd", res);
		return;
	}
	buf[res] = '\0';
	GTRACE("cmd=%s", buf);

	FILE* fp = popen(buf, "r");
	if (fp == nullptr) {
		GTRACE("fail to popen(%s)", buf);
		exit(1);
	}

	char result[BUFSIZE];
	while (::fgets(result, BUFSIZE, fp) != nullptr) {
		size_t len = strlen(result);
		ssize_t writeLen = ::send(sd, result, len, 0);
		if (writeLen == 0 || writeLen == -1) {
			GTRACE("send return %d", (int)writeLen);
			break;
		}
	}
	pclose(fp);
	::close(sd);
}

int main(int argc, char* argv[]) {
	if (!param.parse(argc, argv)) {
		return -1;
	}

	gtrace_default(nullptr, 0, true, nullptr);

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
	_acceptSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_acceptSocket == -1) {
		GTRACE("socket return -1 %s %d", strerror(errno), errno);
		return -1;
	}

#ifdef __linux__
	//
	// setsockopt
	//
	{
		int optval = 1;
		int res = ::setsockopt(_acceptSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
		if (res == -1) {
			GTRACE("setsockopt return -1 %s %d", strerror(errno), errno);
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
		addr.sin_addr.s_addr = ntohl(0x7F090909); // 127.9.9.9
		addr.sin_port = htons(param.port_);

		ssize_t res = ::bind(_acceptSocket, (struct sockaddr *)&addr, sizeof(addr));
		if (res == -1) {
			GTRACE("bind return -1 %s %d", strerror(errno), errno);
			return -1;
		}
	}

	//
	// listen
	//
	{
		int res = listen(_acceptSocket, 5);
		if (res == -1) {
			GTRACE("listen return -1 %s %d", strerror(errno), errno);
			return -1;
		}
	}

	while (true) {
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int newsd = ::accept(_acceptSocket, (struct sockaddr *)&addr, &len);
		if (newsd == -1) {
			GTRACE("accept return -1 %s %d", strerror(errno), errno);
			break;
		}

		std::thread* t = new std::thread(process, newsd);
		t->detach();
	}
	::close(_acceptSocket);

	GTRACE("sudemon terminated successfully\n");
}
