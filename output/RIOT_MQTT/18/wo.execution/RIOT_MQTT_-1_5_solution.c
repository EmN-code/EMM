#include "ztimer.h"
#include "msg.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "MQTTClient.h"

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U)
#define MQTT_TOPIC "state"
#define MQTT_STATE_MESSAGE "work"

static int sock = -1;
static struct sockaddr_in broker_addr;

void publish_state(void) {
    char payload[] = MQTT_STATE_MESSAGE;
    ssize_t sent;

    /* Send the MQTT state message */
    sent = sendto(sock, payload, sizeof(payload), 0,
                  (struct sockaddr *)&broker_addr, sizeof(broker_addr));
    if (sent < 0) {
        /* Handle the error */
        perror("sendto");
    }
    else {
        printf("Published state\n");
    }
}

int init_socket(void) {
    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    /* Configure broker address */
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_SERVER_PORT);
    if (inet_pton(AF_INET, MQTT_SERVER_IP, &broker_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return -1;
    }

    return 0;
}

int main(void) {
    if (init_socket() < 0) {
        return 1;
    }

    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC, &timer);
    ztimer_set(ZTIMER_MSEC, &timer, MQTT_PUBLISH_INTERVAL * 1000);

    while (1) {
        ztimer_wait(&timer);
        publish_state();
    }

    return 0;
}