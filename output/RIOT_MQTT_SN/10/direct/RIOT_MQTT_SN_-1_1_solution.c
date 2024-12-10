#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define MQTT_PORT        1883
#define MQTT_BROKER      "47.102.103.1"
#define PUB_TOPIC        "state"
#define WORK_OR_NOT_WORK "work"
#define PUB_INTERVAL     (5U)

static emcute_sock_t sock;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(MQTT_PORT, stack, sizeof(stack));
    return NULL;
}

static int publish(const char *topic, const char *data) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    /* get topic id */
    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return 1;
    }

    /* publish data */
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }

    printf("Published %s on topic %s
", data, topic);
    return 0;
}

int main(void) {
    // Initialize MQTT-SN connection
    sock = sock_udp_create(NULL, NULL, NULL, NULL);
    if (!sock) {
        puts("Error: unable to create socket");
        return 1;
    }

    if (emcute_soc_init(&sock, MQTT_BROKER, MQTT_PORT) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    puts("Successfully connected to broker");

    char mqtt_data[16];
    sprintf(mqtt_data, "work");

    while (1) {
        publish(PUB_TOPIC, mqtt_data);
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}