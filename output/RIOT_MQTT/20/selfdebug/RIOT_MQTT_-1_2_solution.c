#include <string.h>
#include <stdio.h>
#include <net/emcute.h>  // Ensure you have the correct header for the MQTT client in RIOT.
#include <net/conn.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <saul_reg.h>
#include <emcute.h>

#define WIFI_SSID     "your_ssid"
#define WIFI_PASS     "your_password"

#define SERVER_ADDR   "47.102.103.1"
#define SERVER_PORT   1883
#define CLIENT_ID     "esp32_client"
#define TOPIC         "device/state"
#define MESSAGE       "work"
#define PAUSE_S       (5U)

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static void connect_and_publish(void) {
    emcute_socket_t sock;
    // Connect to the server
    if (sock_udp_create(&sock, &server, NULL, SOCK_FLAGS_REUSE_PORT) < 0) {
        printf("Error creating socket\n");
        return;
    }

    while (1) {
        int res = sock_udp_connect(&sock, &server);
        if (res < 0) {
            printf("Could not connect to broker: %d\n", res);
            continue;
        }
        printf("Connected\n");

        // Publish to a topic
        sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), NULL);
        printf("Published: %s\n", MESSAGE);

        // Wait for the next publish interval
        ztimer_sleep(ZTIMER_MSEC, PAUSE_S * 1000);
    }
}

int main(void) {
    // Initialize address
    inet_pton(AF_INET, SERVER_ADDR, &server.addr);

    // Start publishing
    connect_and_publish();

    return 0;
}