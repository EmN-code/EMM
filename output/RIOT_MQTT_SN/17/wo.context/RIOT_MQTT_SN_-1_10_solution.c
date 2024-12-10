#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include "emcute.h"

// Define the necessary constants
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC_NAME "state"
#define PUB_INTERVAL 5

// Global variables
enum { MAX_SUBSCRIPTIONS = 1 };
static emcute_sub_t subscriptions[MAX_SUBSCRIPTIONS];
static char *status = "work";

static void *publisher_function(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    topic.name = PUB_TOPIC_NAME;
    topic.id = 0;

    size_t len = strlen(status);
    while (1) {
        if (emcute_pub(&topic, status, len, EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error while publishing data\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);  // Corrected implicit declaration

    printf("Connecting to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to gateway at %s:%d\n", BROKER_IP, BROKER_PORT);
        return 1;
    }

    // Start publisher thread
    kernel_pid_t pub_thread = thread_create(stack_pub_thread, sizeof(stack_pub_thread),
                                            THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                            publisher_function, NULL, "publish");
    if (pub_thread <= KERNEL_PID_UNDEF) {
        printf("[-] Could not start publisher thread\n");
        return 1;
    }

    // Emcute runs indefinitely, our main thread will do so too to keep the program alive
    emcute_run(BROKER_PORT, "esp32");
    return 0;
}