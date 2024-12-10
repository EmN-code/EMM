#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <esp_system.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define INTERVAL 5

int main(void) {
    printf("Starting RIOT MQTT Client example...\n");

    // Initialize socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    while (1) {
        const char *msg = "work";
        printf("Publishing message: %s\n", msg);

        if (sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto");
            break;
        }

        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    close(sock);
    return 0;
}