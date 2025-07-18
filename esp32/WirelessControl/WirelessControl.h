#ifndef WIRELESSCONTROL_H
#define WIRELESSCONTROL_H

#include <Arduino.h>

#include "Logger.h"

// How long to wait for a connection
#define WIFI_CONNECT_WAIT 5000

class WirelessControl {
    public:

    // No need for a constructor in a class with only static methods and variables
    
    static void init_wifi(const char *ssid, const char *passwd, const char *hostname);
    static bool connect();
    static void listNetworks();
    static void monitor();

    static bool is_connected;

    static bool is_error_status(uint8_t status);
    static bool is_success_status(uint8_t status);
    static bool is_neutral_status(uint8_t status);

    static int8_t get_rssi();

    private:
    static void printStatus(uint8_t status);
    static void printEncryptionType(int thisType);
};

#endif