#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <iostream>
#include <thread>

using namespace std;

struct Param {
	string command_;

	bool parse(int argc, char* argv[]) {
		if (argc == 2 && strcmp(argv[1], "--version") == 0) {
			usage();
			exit(0);
		}

		if (argc >= 3 && strcmp(argv[1], "-c") == 0) {
			fprintf(stderr, "%s\n", argv[2]); // gilgil temp 2026.03.01
			command_ = argv[2];
		}

		return true;
	}

	static void usage() {
		printf("su version %s\n",
#include "../version.txt"
		);
	}
} param;

void process(int sd) {
	fflush(stdout);
	static const int BUFSIZE = 65536;
	char buf[BUFSIZE];
	while (true) {
		ssize_t res = ::recv(sd, buf, BUFSIZE - 1, 0);
		if (res == 0 || res == -1) {
			fprintf(stderr, "recv return %zd %s %d\n", res, strerror(errno), errno);
			break;
		}
		buf[res] = '\0';
		printf("%s", buf);
		fflush(stdout);
	}
	fprintf(stderr, "1111\n"); // gilgil temp 2026.03.01
	fflush(stdout);
	fprintf(stderr, "222\n"); // gilgil temp 2026.03.01
	::close(sd);
	fprintf(stderr, "333\n"); // gilgil temp 2026.03.01
	pthread_exit(NULL);
	fprintf(stderr, "after exit 44444\n"); // gilgil temp 2026.03.01
}

int main(int argc, char* argv[]) {
	if (!param.parse(argc, argv))
		return -1;

	//
	// socket
	//
	int sd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		fprintf(stderr, "socket return -1 %s %d\n", strerror(errno), errno);
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
			fprintf(stderr, "setsockopt return -1 %s %d\n", strerror(errno), errno);
			return -1;
		}
	}
#endif // __linux

	//
	// connect
	//
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(0x7F090909); // 127.9.9.9
	addr.sin_port = htons(9999);
	{
		int res = ::connect(sd, (struct sockaddr*)&addr, sizeof(addr));
		if (res == -1) {
			fprintf(stderr, "connect return -1 %s %d\n", strerror(errno), errno);
			return -1;
		}
	}

	thread t(process, sd);
	t.detach();

	if (param.command_ != "") {
		string cmd = param.command_;
		cmd += "\n";
		ssize_t res = ::send(sd, cmd.data(), cmd.size(), 0);
		if (res == 0 || res == -1) {
			fprintf(stderr, "send return %zd %s %d\n", res, strerror(errno), errno);
		}
		this_thread::sleep_for(1s); // gilgil temp 2026.03.01
	} else {
		while (true) {
			string cmd;
			fprintf(stderr, "bef getline\n"); // gilgil temp 2026.03.01
			getline(cin, cmd);
			fprintf(stderr, "aft getline\n"); // gilgil temp 2026.03.01
			if (cmd == "exit") break;
			cmd += "\n";
			ssize_t res = ::send(sd, cmd.data(), cmd.size(), 0);
			if (res == 0 || res == -1) {
				fprintf(stderr, "send return %zd %s %d\n", res, strerror(errno), errno);
				break;
			}
		}
	}
	::close(sd);
	fprintf(stderr, "terminated successfully\n");
}
