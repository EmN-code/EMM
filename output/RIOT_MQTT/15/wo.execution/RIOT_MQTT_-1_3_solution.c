#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC "device/status"
#define BUF_SIZE 128

void publish_state(int sock, struct sockaddr_in *broker_addr) {
    const char *states[2] = {"work", "not work"};
    int state = 0;

    while (1) {
        char buffer[BUF_SIZE];
        snprintf(buffer, sizeof(buffer), "{\"state\": \"%s\"}", states[state]);
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)broker_addr, sizeof(*broker_addr));
        state = !state;
        sleep(5);
    }
}

int main(void) {
    int sock;
    struct sockaddr_in broker_addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &broker_addr.sin_addr);

    publish_state(sock, &broker_addr);

    close(sock);
    return 0;
}