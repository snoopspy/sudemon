#include "gsignal.h"
#include <cstdio>
#include <cstdlib>

// ----------------------------------------------------------------------------
// GSignal
// ----------------------------------------------------------------------------
GSignal::GSignal() {
}

GSignal::~GSignal() {
	for (HandlerMap::iterator it = handlerMap_.begin(); it != handlerMap_.end(); it++) {
		int signo = it->first;
		Handler oldHandler = it->second.oldHandler_;
		std::signal(signo, oldHandler);
	}
}

void GSignal::mySignalHandler(int signo) {
	HandlerMap::iterator it = handlerMap_.find(signo);
	if (it == handlerMap_.end()) {
		fprintf(stderr, "can not find signal %d\n", signo);
		exit(-1);
	}

	Handler newHandler = it->second.newHandler_;
	newHandler(signo);

	if (callOriginFunc_) {
		Handler oldHandler = it->second.oldHandler_;
		oldHandler(signo);
	}

	if (signalOnce_) {
		Handler oldHandler = it->second.oldHandler_;
		std::signal(signo, oldHandler);
		handlerMap_.erase(it);
	}
}

void GSignal::_mySignalHandler(int signo) {
	GSignal& signal = GSignal::instance();
	signal.mySignalHandler(signo);
}

void GSignal::setup(int signo, Handler newHandler, void* user) {
	Handlers handlers;
	handlers.newHandler_ = newHandler;
	handlers.user_ = user;
	handlers.oldHandler_ = std::signal(signo, _mySignalHandler);
	handlerMap_[signo] = handlers;
}

void GSignal::setupAll(Handler newHandler, void* user) {
	setup(SIGINT, newHandler, user);
	setup(SIGILL, newHandler, user);
	setup(SIGABRT, newHandler, user);
	setup(SIGFPE, newHandler, user);
	setup(SIGSEGV, newHandler, user);
#ifndef WIN32
	setup(SIGTERM, newHandler, user);
	setup(SIGHUP, newHandler, user);
	setup(SIGQUIT, newHandler, user);
	setup(SIGTRAP, newHandler, user);
	setup(SIGKILL, newHandler, user);
	setup(SIGBUS, newHandler, user);
	setup(SIGSYS, newHandler, user);
	ignore(SIGPIPE); // ignore SIGPIPE which can be signaled when TCP socket operation on linux
	setup(SIGALRM, newHandler, user);
#endif
}

void GSignal::ignore(int signo) {
	std::signal(signo, SIG_IGN);
}

void* GSignal::getUser(int signo) {
	HandlerMap::iterator it = handlerMap_.find(signo);
	if (it == handlerMap_.end())
		return nullptr;
	void* user = it->second.user_;
	return user;
}

const char* GSignal::getString(int signo) {
	const char* signal = "unknown";
	switch (signo) {
		case SIGINT: signal = "SIGINT"; break;
		case SIGILL: signal = "SIGILL"; break;
		case SIGABRT: signal = "SIGABRT"; break;
		case SIGFPE: signal = "SIGFPE"; break;
		case SIGSEGV: signal = "SIGSEGV"; break;
		case SIGTERM: signal = "SIGTERM"; break;
#ifndef WIN32
		case SIGHUP: signal = "SIGHUP"; break;
		case SIGQUIT: signal = "SIGQUIT"; break;
		case SIGTRAP: signal = "SIGTRAP"; break;
		case SIGKILL: signal = "SIGKILL"; break;
		case SIGBUS: signal = "SIGBUS"; break;
		case SIGSYS: signal = "SIGSYS"; break;
		case SIGPIPE: signal = "SIGPIPE"; break;
		case SIGALRM: signal = "SIGALRM"; break;
		case SIGURG: signal = "SIGURG"; break;
		case SIGSTOP: signal = "SIGSTOP"; break;
		case SIGTSTP: signal = "SIGTSTP"; break;
		case SIGCONT: signal = "SIGCONT"; break;
		case SIGCHLD: signal = "SIGCHLD"; break;
		case SIGTTIN: signal = "SIGTTIN"; break;
		case SIGTTOU: signal = "SIGTTOU"; break;
		case SIGPOLL: signal = "SIGPOLL"; break;
		case SIGXCPU: signal = "SIGXCPU"; break;
		case SIGXFSZ: signal = "SIGXFSZ"; break;
		case SIGVTALRM: signal = "SIGVTALRM"; break;
		case SIGPROF: signal = "SIGPROF"; break;
		case SIGUSR1: signal = "SIGUSR1"; break;
		case SIGUSR2: signal = "SIGUSR2"; break;
#endif
	}
	return signal;
}
