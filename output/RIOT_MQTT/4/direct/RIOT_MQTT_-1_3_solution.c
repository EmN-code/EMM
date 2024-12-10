#include <stdio.h>
#include <string.h>

#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IPV6_ADDR     "47.102.103.1"
#define BROKER_PORT          1883
#define MQTT_TOPIC           "state"
#define MQTT_MESSAGE_WORK    "work"
#define MQTT_MESSAGE_NOTWORK "not work"
#define SENDING_INTERVAL     (5U * US_PER_SEC)

static emcute_sock_t sock;
static uint8_t stack[THREAD_STACKSIZE_MAIN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Received data on topic '%s': %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void)
{
    ipv6_addr_t addr;
    emcute_topic_t t;
    emcute_sub_t subscriptions[1];
    char * topic_name = MQTT_TOPIC;
    
    // Initialize network
    printf("Connecting to broker %s with port %d...\n", BROKER_IPV6_ADDR, BROKER_PORT);
    if (ipv6_addr_from_str(&addr, BROKER_IPV6_ADDR) == NULL) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    // Setup EMCUTE
    sock = emcute_create_sock();
    if (emcute_sock_open(&sock, &addr, sizeof(addr), BROKER_PORT) != EMCUTE_OK) {
        puts("Error: unable to open socket");
        return 1;
    }

    // Start the subscription
    subscriptions[0].cb = on_pub;
    subscriptions[0].topic.name = topic_name;
    emcute_sub(&sock, &subscriptions[0], 1);

    // Set topic
    t.name = topic_name;

    while (1) {
        char *msg = (rand()%2) == 0 ? MQTT_MESSAGE_WORK : MQTT_MESSAGE_NOTWORK;
        printf("Publishing message: %s\n", msg);
        emcute_pub(&sock, &t, msg, strlen(msg), EMCUTE_QOS_0);
        xtimer_sleep(SENDING_INTERVAL / US_PER_SEC);
    }

    emcute_sock_close(&sock);
    return 0;
}