#include <string.h>
#include <thread.h>
#include <stdio.h>
#include <stdint.h>
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_PORT     (1883U)
#define BROKER_IP       "47.102.103.1"
#define TOPIC_NAME      "device/state"
#define PUBLISH_INTERVAL (5000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[1];

static void *thread_entry(void *arg) {
    (void)arg;
    emcute_shell_init();
    emcute_run(NULL, EMCUTE_PORT);
    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *msg = (char *)data;
    msg[len] = '\0';
    printf("Received publication: %s [%s]\n", topic->name, msg);
}

int main(void) {
    puts("Initializing MQTT-SN publisher for ESP32\n");

    // Initialize thread for MQTT-SN
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  thread_entry, NULL, "emcute");

    // Create a topic
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = TOPIC_NAME;
    t.id = 0; // set by emcute_reg

    // Setup the subscriptions
    subscriptions[0].cb = on_pub;
    strcpy(subscriptions[0].topic.name, t.name);
    subscriptions[0].topic.id = t.id;
    subscriptions[0].topic.flags = flags;

    // Connect to the broker
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    ipv4_addr_t addr;
    ipv4_addr_from_str(&addr, BROKER_IP);
    memcpy(&gw.addr.ipv4, &addr, sizeof(addr));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to the broker\n");
        return 1;
    }

    puts("Connected to the broker\n");

    // Register the topic
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Failed to register topic\n");
        return 1;
    }

    // Publishing loop
    while (1) {
        const char *message = "work";
        if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
            puts("Failed to publish message\n");
        }
        else {
            puts("Published: work\n");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from broker
    emcute_discon();
    return 0;
}