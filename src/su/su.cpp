#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <string>

struct Param {
	std::string command_;

	bool parse(int argc, char* argv[]) {
		if (argc < 3) {
			usage();
			return false;
		}

		if (strcmp(argv[1], "-c") != 0) {
			usage();
			return false;
		}

		command_ = argv[2];

		return true;
	}

	static void usage() {
		printf("su version %s\n",
#include "../version.txt"
		);
		printf("Copyright (c) Gilbert Lee All rights reserved\n");
		printf("\n");
		printf("syntax : su -c <command>\n");
		printf("sample : su -c whoami\n");
	}
} param;

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

	ssize_t res = ::send(sd, param.command_.data(), param.command_.size(), 0);
	if (res == 0 || res == -1) {
		fprintf(stderr, "send return %zd %s %d\n", res, strerror(errno), errno);
	}

	fflush(stdout);
	static const int BUFSIZE = 65536;
	char buf[BUFSIZE];
	while (true) {
		ssize_t res = ::recv(sd, buf, BUFSIZE - 1, 0);
		if (res == 0 || res == -1) {
			// fprintf(stderr, "recv return %zd %s %d\n", res, strerror(errno), errno);
			break;
		}
		buf[res] = '\0';
		printf("%s", buf);
		fflush(stdout);
	}
	::close(sd);
	// fprintf(stderr, "terminated successfully\n");
}
