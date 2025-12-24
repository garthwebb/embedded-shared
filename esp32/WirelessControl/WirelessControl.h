#ifndef WIRELESSCONTROL_H
#define WIRELESSCONTROL_H

#include <Arduino.h>

#include "Logger.h"

// How long to wait for a connection
#define WIFI_CONNECT_WAIT_MS 10000
#define WIFI_CONNECT_POLL_MS 500

class WirelessControl {
    public:

    // No need for a constructor in a class with only static methods and variables

    static void init_wifi(const char *ssid, const char *passwd, const char *hostname);
    static void init_wifi_from_list(const char *const *credentials, size_t count, const char *hostname);

    // Template overload that deduces the number of elements in a fixed C-style array
    // so callers can pass an array literal or macro like `WIFI_CREDENTIALS_LIST`
    template <size_t N>
    static void init_wifi_from_list(const char *const (&credentials)[N], const char *hostname) {
        init_wifi_from_list(credentials, N, hostname);
    }
    static void reset_connection();
    static bool block_until_connect();
    static void listNetworks();
    static void monitor();

    static bool is_connected;
    static const char *_ssid;
    static const char *_passwd;

    static bool is_error_status(uint8_t status);
    static bool is_success_status(uint8_t status);
    static bool is_neutral_status(uint8_t status);

    static int8_t get_rssi();

    private:
    static void printStatus(uint8_t status);
    static void printEncryptionType(int thisType);
};

#endif