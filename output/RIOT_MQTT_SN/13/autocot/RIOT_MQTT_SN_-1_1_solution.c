#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "random.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/conn/udp.h"

#define EMCUTE_ID           "ESP32"
#define EMCUTE_PORT         1883
#define MQTT_TOPIC_NAME     "state"
#define MQTT_BROKER_IP      "47.102.103.1"
#define PUB_INTERVAL        (5U * 1000000U)     // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void *thread_pub(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC_NAME;
    topic.id = 0; // Assuming the gateway assigns IDs

    while(1) {
        const char* message = "work";  // State message
        // Publish the message
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }
        // Wait for some time before the next publish
        xtimer_sleep(PUB_INTERVAL / 1000000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    // Start the thread that handles publishing
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_pub, NULL, "pub");

    // Setup emcute
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    sock_udp_ep_t gw = { .family = AF_INET,
                         .port = EMCUTE_PORT };
    if (gnrc_netif_ipv4_get(_netif, NULL, -1) < 0) {
        puts("ERROR: unable to configure netif with IPv4");
        return -1;
    }
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // Connect to defined MQTT broker
    if (emcute_con(&gw, false, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    while(1) {
        // Keep the main thread alive
        xtimer_sleep(1);
    }

    return 0;
}