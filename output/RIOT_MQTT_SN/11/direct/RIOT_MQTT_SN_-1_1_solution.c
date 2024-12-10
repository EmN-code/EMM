#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_socket_t sock;

void *publisher(void *arg) {
    (void) arg;
    emcute_topic_t t;
    char *topic_name = MQTT_TOPIC;
    char pub_data[16];
    snprintf(pub_data, sizeof(pub_data), "work"); // Use "work" or "not work"

    t.name = topic_name;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: Unable to register topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&t, pub_data, strlen(pub_data), 0) != EMCUTE_OK) {
            puts("Error: Unable to publish data");
        } else {
            printf("Published: %s\n", pub_data);
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN Publisher Example");

    sock.remote.family = AF_INET;
    sock.remote.netif = SOCK_ADDR_ANY_NETIF;
    sock.local.port = 0;  // Let RIOT choose the port
    sock.remote.port = BROKER_PORT;
    ipv6_addr_from_str(&sock.remote.addr.ipv6, BROKER_IP);

    if (emcute_con(&sock, true, MQTT_TOPIC, "Hello World") != EMCUTE_OK) {
        puts("Error: Unable to connect to broker");
        return 1;
    }

    puts("Connected to broker");

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher, NULL, "publisher");

    xtimer_sleep(UINT32_MAX);
    return 0;
}
