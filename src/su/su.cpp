#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef __linux__
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif // __linux
#include <iostream>
#include <thread>

using namespace std;

void myerror(const char* msg) { fprintf(stderr, "%s %s %d\n", msg, strerror(errno), errno); }

struct Param {
	string command_;

	bool parse(int argc, char* argv[]) {
		if (argc == 2 && strcmp(argv[1], "--version") == 0) {
			usage();
			exit(0);
		}

		if (argc > 3 && strcmp(argv[1], "-c") == 0)
			command_ = stoi(argv[2]);

		return true;
	}

	void usage() {
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
			fprintf(stderr, "recv return %zd", res);
			myerror(" ");
			break;
		}
		buf[res] = '\0';
		printf("%s", buf);
		fflush(stdout);
	}
	fflush(stdout);
	::close(sd);
	exit(0);
}

int main(int argc, char* argv[]) {
	if (!param.parse(argc, argv))
		return -1;

	//
	// socket
	//
	int sd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		fprintf(stderr, "socket return -1 %s %d", strerror(errno), errno);
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
	// connect
	//
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(0x7F090909); // 127.9.9.9
	addr.sin_port = htons(9999);
	{
		int res = ::connect(sd, (struct sockaddr*)&addr, sizeof(addr));
		if (res == -1) {
			myerror("connect");
			return -1;
		}
	}

	std::thread t(process, sd);
	t.detach();

	while (true) {
		std::string s;
		std::getline(std::cin, s);
		if (s == "exit") break;
		s += "\r\n";
		ssize_t res = ::send(sd, s.data(), s.size(), 0);
		if (res == 0 || res == -1) {
			fprintf(stderr, "send return %zd", res);
			myerror(" ");
			break;
		}
	}
	::close(sd);
}
