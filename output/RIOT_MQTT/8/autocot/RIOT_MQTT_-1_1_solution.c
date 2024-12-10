#include "udp.h"
#include "thread.h"
#include "timex.h"
#include "xtimer.h"
#include "net/sock/udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 128

static char buffer[BUF_SIZE];

void *mqtt_publish_thread(void *arg) {
    (void)arg;  // unused parameter
    sock_udp_t sock;
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    uint8_t state = 0;  // Initialize the state to "not work"

    printf("Connecting to UDP server at %s:%d\n", SERVER_ADDR, SERVER_PORT);

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket");
        return NULL;
    }

    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) != 1) {
        puts("Error: Unable to convert IP address");
        return NULL;
    }

    while (1) {
        const char *state_str = state ? "work" : "not work";
        snprintf(buffer, BUF_SIZE, "State: %s", state_str);

        if (sock_udp_send(&sock, buffer, strlen(buffer), &server) < 0) {
            puts("Error: Unable to send message");
            xtimer_sleep(1);  // Wait a bit before retrying
            continue;
        }

        printf("Sent: %s\n", buffer);
        state = !state;  // Toggle state between "work" and "not work"
        xtimer_sleep(5);  // send every 5 seconds
    }

    return NULL;
}

int main(void) {
    thread_create(buffer, sizeof(buffer), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publish_thread, NULL,
                  "mqtt_publish_thread");

    return 0;
}