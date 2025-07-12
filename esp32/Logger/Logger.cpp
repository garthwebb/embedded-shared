#include <Logger.h>
#include <memory>
#include "WirelessControl.h"

WiFiUDP udpClient;

// Create a new syslog instance with LOG_KERN facility
Syslog *syslog = nullptr;

void Logger::init(const char *server, uint16_t port, const char *hostname, const char *app_name) {
	syslog = new Syslog(udpClient, server, port, hostname, app_name, LOG_KERN);
}

bool Logger::log(String msg) {
	return log_info(msg);
}

bool Logger::log_info(String msg) {
	if (!WirelessControl::is_connected) {
		// Don't log if we don't have a connection
		Serial.println("WiFi disconnected: Syslog serial fallback [info]: " + msg);
		return false;
	}

#ifdef LOG_TO_SYSLOG
	// Exit here if LOG_TO_SYSLOG is defined and false
	if (!LOG_TO_SYSLOG) { Serial.printf("[INFO] %s\n", msg.c_str()); return true; }
#endif

		return syslog->log(LOG_INFO, msg.c_str());
}

bool Logger::log_error(String msg) {
	if (!WirelessControl::is_connected) {
		// Don't log if we don't have a connection
		Serial.println("WiFi disconnected: Syslog serial fallback [error]: " + msg);
		return false;
	}

#ifdef LOG_TO_SYSLOG
	// Exit here if LOG_TO_SYSLOG is defined and false
	if (!LOG_TO_SYSLOG) { Serial.printf("[ERROR] %s\n", msg.c_str()); return true; }
#endif

	return syslog->log(LOG_ERR, msg.c_str());
}

bool Logger::log_debug(String msg) {
	if (!WirelessControl::is_connected) {
		// Don't log if we don't have a connection
		Serial.println("WiFi disconnected: Syslog serial fallback [debug]: " + msg);
		return false;
	}

#ifdef LOG_TO_SYSLOG
	// Exit here if LOG_TO_SYSLOG is defined and false
	if (!LOG_TO_SYSLOG) { Serial.printf("[DEBUG] %s\n", msg.c_str()); return true; }
#endif

	return syslog->log(LOG_DEBUG, msg.c_str());
}

bool Logger::log_warn(String msg) {
	if (!WirelessControl::is_connected) {
		// Don't log if we don't have a connection
		Serial.println("WiFi disconnected: Syslog serial fallback [warn]: " + msg);
		return false;
	}	
#ifdef LOG_TO_SYSLOG
	// Exit here if LOG_TO_SYSLOG is defined and false
	if (!LOG_TO_SYSLOG) { Serial.printf("[WARN] %s\n", msg.c_str()); return true; }
#endif	

	return syslog->log(LOG_WARNING, msg.c_str());
}