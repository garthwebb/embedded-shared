#include <WirelessControl.h>
#include <WiFi.h>

extern Logger *LOGGER;

bool WirelessControl::is_connected = false;
const char *WirelessControl::_ssid = nullptr;
const char *WirelessControl::_passwd = nullptr;

void WirelessControl::init_wifi(const char *ssid, const char *passwd, const char *hostname) {
    WiFi.setHostname(hostname);
    _ssid = ssid;
    _passwd = passwd;

    listNetworks();

    Serial.println("Connecting to " + String(ssid) + ":");

    WiFi.begin(_ssid, _passwd);

	while (!block_until_connect()) {
        reset_connection();
    }

    is_connected = true;
    Serial.println("Connected to " + String(ssid) + " with IP " + WiFi.localIP().toString());
}

void WirelessControl::reset_connection() {
    Serial.println("WiFi disconnected, trying to reconnect...");
    WiFi.disconnect();
    // reassert STA mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _passwd);
}

bool WirelessControl::block_until_connect() {
    int status = WiFi.status();
    unsigned long start_time = millis();
    unsigned long end_time = start_time + WIFI_CONNECT_WAIT_MS;

    // Block until we're etther connected or we time out
    while ((status != WL_CONNECTED) && (millis() < end_time)) {
        // Give a little time between checks
        delay(WIFI_CONNECT_POLL_MS);
        status = WiFi.status();
    }

    if (is_error_status(status)) {
        printStatus(status);
        return false;
    }

    return true;
}

void WirelessControl::monitor() {
	uint8_t status = WiFi.status();
	bool was_connected = is_connected;

	// Return immediately if we're still connected
	if (status == WL_CONNECTED) {
		return;
	}

    // We are no longer connected
    is_connected = false;

    // Keep track of how long it takes to reconnect, and reset thhe connection
    uint32_t reconnect_start_time = millis();
	reset_connection();

    // Block here until we get a connection otherwise, let the watchdog reset us
	while (!block_until_connect()) {
        reset_connection();
    }

	// Log that we lost a connection but are now reconnected
	if (was_connected) {
        LOGGER->log_error("WiFi connection was lost: took " + String((millis() - reconnect_start_time) / 1000.0, 2) + "s to reconnect.");
	}

    LOGGER->log("Connected to " + WiFi.SSID() + " with IP " + WiFi.localIP().toString());
	Serial.println("Connected to " + WiFi.SSID() + " with IP " + WiFi.localIP().toString());
 	is_connected = true;
}

void WirelessControl::listNetworks() {
    // scan for nearby networks:
    Serial.println("** Scanning Networks **");
    int numSsid = WiFi.scanNetworks(false, false, false, 5000); // 5s timeout
    if (numSsid == -1) {
        Serial.println("Couldn't get a wifi connection");
        return;
    }

    // print the list of networks seen:
    Serial.print("Number of available networks: ");
    Serial.println(numSsid);

    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        Serial.print(thisNet);
        Serial.print(") ");
        Serial.print(WiFi.SSID(thisNet));
        Serial.print("\tSignal: ");
        Serial.print(WiFi.RSSI(thisNet));
        Serial.print(" dBm");
        Serial.print("\tEncryption: ");
        printEncryptionType(WiFi.encryptionType(thisNet));
    }
}

void WirelessControl::printEncryptionType(int thisType) {
    // read the encryption type and print out the name:
    switch (thisType) {
        case WIFI_AUTH_OPEN:             /**< authenticate mode : open */
            Serial.println("Open");
            break;
        case WIFI_AUTH_WEP:              /**< authenticate mode : WEP */
            Serial.println("WEP");
            break;
        case WIFI_AUTH_WPA_PSK:          /**< authenticate mode : WPA_PSK */
            Serial.println("WPA_PSK");
            break;
        case WIFI_AUTH_WPA2_PSK:         /**< authenticate mode : WPA2_PSK */
            Serial.println("WPA2_PSK");
            break;
        case WIFI_AUTH_WPA_WPA2_PSK:     /**< authenticate mode : WPA_WPA2_PSK */
            Serial.println("WPA_WPA2_PSK");
            break;
        case WIFI_AUTH_WPA2_ENTERPRISE:  /**< authenticate mode : WPA2_ENTERPRISE */
            Serial.println("WPA2_ENTERPRISE");
            break;
        case WIFI_AUTH_WPA3_PSK:         /**< authenticate mode : WPA3_PSK */
            Serial.println("WPA3_PSK");
            break;
        case WIFI_AUTH_WPA2_WPA3_PSK:    /**< authenticate mode : WPA2_WPA3_PSK */
            Serial.println("WPA2_WPA3_PSK");
            break;
        case WIFI_AUTH_WAPI_PSK:         /**< authenticate mode : WAPI_PSK */
            Serial.println("WAPI_PSK");
            break;
        case WIFI_AUTH_MAX:
            Serial.println("MAX");
            break;
        default:
            Serial.print("unknown=");
            Serial.println(thisType);
    }
}

void WirelessControl::printStatus(uint8_t status) {
    switch (status) {
        case WL_NO_SHIELD:
            Serial.println("\tResult: No sheild");
            break;
        case WL_IDLE_STATUS:
            Serial.println("\tResult: Idle");
            break;
        case WL_NO_SSID_AVAIL:
            Serial.println("\tResult: No SSID available");
            break;
        case WL_SCAN_COMPLETED:
            Serial.println("\tResult: Scan completed");
            break;
        case WL_CONNECTED:
            Serial.println("\tResult: Connected");
            break;
        case WL_CONNECT_FAILED:
            Serial.println("\tResult: Connect failed");
            break;
        case WL_CONNECTION_LOST:
            Serial.println("\tResult: Connection lost");
            break;
        case WL_DISCONNECTED:
            Serial.println("\tResult: disconnected");
            break;
        default:
            Serial.println("\tResult: unknown return");
    }
}

bool WirelessControl::is_error_status(uint8_t status) {
    if (status == WL_NO_SSID_AVAIL || status == WL_CONNECT_FAILED || status == WL_CONNECTION_LOST) {
        return true;
    }
    return false;
}

bool WirelessControl::is_success_status(uint8_t status) {
    if (status == WL_CONNECTED) {
        return true;
    }
    return false;
}

bool WirelessControl::is_neutral_status(uint8_t status) {
    if (status == WL_IDLE_STATUS || status == WL_SCAN_COMPLETED || WL_DISCONNECTED) {
        return true;
    }
    return false;
}

int8_t WirelessControl::get_rssi() {
    return WiFi.RSSI();
}
