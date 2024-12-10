#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC         "work_status"
#define MQTT_BROKER_IPV6   "47.102.103.1"
#define DELAY              (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

typedef struct {
    char *host;
    uint16_t port;
} mqtt_broker_t;

static mqtt_broker_t broker = {MQTT_BROKER_IPV6, EMCUTE_PORT};

static emcute_topic_t subscription;
static char pub_data[64];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, &queue[0], sizeof(queue));
    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)data;
    (void)len;
    printf("Published to topic '%s'\n", topic->name);
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, broker.host);
    gw.port = broker.port;

    if (emcute_con(&gw, true, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }
    printf("Successfully connected to [mqtt://%s]:%u\n", broker.host, broker.port);

    subscription.name = MQTT_TOPIC;
    if (emcute_sub(&subscription, EMCUTE_QOS_0, on_pub, NULL) != EMCUTE_OK) {
        puts("error: unable to subscribe to topic\n");
        return 1;
    }
    printf("Subscribed to topic '%s'\n", MQTT_TOPIC);

    return 0;
}

int main(void) {
    puts("ESP32 MQTT client\n");

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    if (mqtt_connect() != 0) {
        puts("error: connection failed\n");
        return 1;
    }

    while (1) {
        snprintf(pub_data, sizeof(pub_data), "work");  // Publishing "work" every 5 seconds

        emcute_topic_t t = { .name = MQTT_TOPIC };      
        if (emcute_pub(&t, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        } else {
            printf("Published work status: %s\n", pub_data);
        }
        xtimer_sleep(DELAY);
    }

    return 0;
}