
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"
#define SEND_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t server = { .family = AF_INET, .port = UDP_SERVER_PORT };
static char device_state[10];

void wifi_init(void) {
    esp_wifi_init();
    esp_wifi_set_mode(WIFI_MODE_STA);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();
}

void send_udp_message(void) {
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;

    if (sock_udp_create(&sock, &local, &server, 0) < 0) {
        puts("Failed to create UDP socket");
        return;
    }

    if (sock_udp_send(&sock, device_state, strlen(device_state), &server) < 0) {
        puts("Failed to send UDP message");
    } else {
        printf("Sent device state: %s\n", device_state);
    }

    sock_udp_close(&sock);
}

int main(void) {
    // Initialize WiFi
    wifi_init();

    // Set initial device state
    strcpy(device_state, DEVICE_STATE_WORK);

    while (1) {
        // Send device state every 5 seconds
        send_udp_message();
        xtimer_usleep(SEND_INTERVAL);

        // Toggle device state
        if (strcmp(device_state, DEVICE_STATE_WORK) == 0) {
            strcpy(device_state, DEVICE_STATE_NOT_WORK);
        } else {
            strcpy(device_state, DEVICE_STATE_WORK);
        }
    }

    return 0;
}
