// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include <csignal>
#include <map>

// ----------------------------------------------------------------------------
// GSignal
// ----------------------------------------------------------------------------
struct GSignal {
private: // singleton
	GSignal();
	virtual ~GSignal();

public:
	bool signalOnce_{true};
	bool callOriginFunc_{false};

public:
#ifdef __linux__
	typedef __sighandler_t Handler;
#endif
#ifdef WIN32
	typedef __p_sig_fn_t Handler;
#endif
protected:
	struct Handlers {
		Handler oldHandler_;
		Handler newHandler_;
		void* user_;
	};
	struct HandlerMap : std::map<int, Handlers> {
	} handlerMap_;
	void mySignalHandler(int signo);
	static void _mySignalHandler(int signo);

public:
	void setup(int signo, Handler newHandler, void* user = nullptr);
	void setupAll(Handler newHandler, void* user = nullptr);
	void ignore(int signo);
	void* getUser(int signo);
	static const char* getString(int signo);

	static GSignal& instance() {
		static GSignal signal;
		return signal;
	}
};
