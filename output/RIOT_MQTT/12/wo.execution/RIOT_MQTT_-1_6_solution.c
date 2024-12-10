#include <ztimer.h>
#include <stdio.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <event/thread.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "status"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static int sock;

void mqtt_publish_task(void) {
    struct sockaddr_in server_addr;
    int len;
    char *message = "work";

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printf("inet_pton error\n");
        return;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("socket creation error\n");
        return;
    }

    while (1) {
        // Send message
        len = sendto(sock, message, strlen(message), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (len < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %s\n", message);
        }

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);

        // Alternate message
        message = (strcmp(message, "work") == 0) ? "not work" : "work";
    }
}

int main(void) {
    printf("MQTT Publish Example\n");
    mqtt_publish_task();
    return 0;
}