#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <event/thread.h>

#define EMCUTE_PORT (1883U)
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT (1883U)
#define PUB_INTERVAL (5U) // Publish every 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

static void *mqtt_publisher_thread(void *arg) {
    (void)arg;
    char message[] = "work";
    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

    // Assume ipv6_address_from_str() conversion function available if needed
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, SERVER_ADDR);

    // Start the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_run, NULL, "emcute");
    
    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Connection failed");
        return 1;
    }

    // Register a topic
    char *topic_name = "state";
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Start the publisher thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, mqtt_publisher_thread, NULL, "pub_thread");

    return 0;
}