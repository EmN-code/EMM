#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/conn_sock_udp.h>

#define MQTT_INTERVAL (5U * 1000000U) // 5 seconds in microseconds
#define MQTT_TOPIC "device/state"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_STATE "work"
#define MQTT_NOT_STATE "not work"

static char *state = MQTT_STATE;

static void *mqtt_pub_thread(void *arg) {
    emcute_sock_t sock = {0};
    sock.sa_family = AF_INET;
    sock.sa_port = MQTT_BROKER_PORT;
    ipv6_addr_from_str(&sock.sa_addr.sin6_addr, MQTT_BROKER_IP);

    emcute_con(&sock, true, NULL, NULL, 0);

    emcute_topic_t topic;
    memset(&topic, 0, sizeof(topic));
    topic.name = MQTT_TOPIC;

    char message[10];

    while (1) {
        
        snprintf(message, sizeof(message), "%s", state);
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);

        printf("Published state: %s\n", message);

        state = (strcmp(state, MQTT_STATE) == 0) ? MQTT_NOT_STATE : MQTT_STATE;

        xtimer_sleep(MQTT_INTERVAL / 1000000U);
    }

    return NULL;
}

int main(void) {
    puts("Connecting to MQTT broker and initiating publish loop...");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_pub_thread, NULL, "mqtt_pub_thread");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}