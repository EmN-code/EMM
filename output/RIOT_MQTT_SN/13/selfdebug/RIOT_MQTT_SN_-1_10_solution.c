// This code demonstrates how to set up an MQTT-SN client on the ESP32 using RIOT's emcute library.

#include <stdio.h>
#include <string.h>
#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME          "state"
#define BROKER_PORT         1883

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];  // Correct declaration of the message queue handling

static emcute_sub_t subscriptions[SUBSCRIPTION_NUM];
static char subscription_stack[THREAD_STACKSIZE_MAIN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("Received publication from topic '%s' [%i]: %.*s\n",
           topic->name, (int)topic->id, (int)len, in);
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Initialize the emcute stack
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_run, NULL, "emcute");

    // Configure the broker address
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, "::ffff:47.102.103.1", &gw.addr);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        return 1;
    }
    printf("Connected to [47.102.103.1]:1883\n");

    // Run the publication loop
    while (1) {
        const emcute_topic_t topic = { .name = TOPIC_NAME, .id = 0 }; // The ID will get auto-populated on publish
        const char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish data\n");
        }
        printf("Published '%s' to topic '%s'\n", message, TOPIC_NAME);
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    // This code will never be reached, but for educational purposes:
    emcute_discon();
    return 0;
}
