#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define BUF_SIZE 128

static sock_udp_ep_t broker;

void publish_state(const char *state) {
    char buf[BUF_SIZE];
    snprintf(buf, sizeof(buf), "{\"state\":\"%s\"}", state);
    sock_udp_send(NULL, buf, strlen(buf), &broker);
}

int main(void) {
    /* Initialize broker endpoint */
    memset(&broker, 0, sizeof(broker));
    broker.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);
    broker.port = MQTT_BROKER_PORT;

    puts("MQTT state publisher started");
    while (1) {
        publish_state("work");
        xtimer_sleep(5);
        publish_state("not work");
        xtimer_sleep(5);
    }
    return 0;
}