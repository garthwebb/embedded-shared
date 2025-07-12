#ifndef LOGGER_H
#define LOGGER_H

#include <Syslog.h>
#include <WiFiUdp.h>
#include <string>

class Logger {
	public:
	Syslog *syslog;

	void init(const char *server, uint16_t port, const char *hostname, const char *app_name);
	bool log(String msg);
	bool log_info(String msg);
	bool log_warn(String msg);
	bool log_error(String msg);
	bool log_debug(String msg);
};

#endif