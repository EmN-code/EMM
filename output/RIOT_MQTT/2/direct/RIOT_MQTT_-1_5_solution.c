#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/state"

static emcute_sock_t sock;
static char mqtt_msg[32];

void *publish_state(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = PUB_TOPIC;
    t.id = 0;
    
    while (1) {
        strcpy(mqtt_msg, "work");
        if (emcute_pub(&t, mqtt_msg, strlen(mqtt_msg), flags) != EMCUTE_OK) {
            printf("[MQTT] Error publishing message\n");
        }
        sleep(5);
    }
    return NULL;
}

int main(void) {
    sock.udp = SOCK_RAW;
    sock.tls = 0;

    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, BROKER_IP) == NULL) {
        printf("[MQTT] Error parsing broker address\n");
        return 1;
    }

    if (emcute_connect(&sock, &addr, BROKER_PORT) != EMCUTE_OK) {
        printf("[MQTT] Error connecting to broker\n");
        return 1;
    }

    printf("[MQTT] Connected to broker\n");

    pthread_t pub_thread;
    pthread_create(&pub_thread, NULL, publish_state, NULL);

    while (1) {
        sleep(1);
    }

    return 0;
}
