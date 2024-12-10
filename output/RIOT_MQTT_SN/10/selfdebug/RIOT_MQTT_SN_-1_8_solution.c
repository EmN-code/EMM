#include <xtimer.h>
#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds
#define TOPIC_NAME "device/status"

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };

static void* pub_thread(void* arg) {
    (void) arg;
    char* states[] = {"work", "not work"};
    int state = 0;
    
    while (1) {
        if (emcute_reg(&topic) != EMCUTE_OK) {
            puts("Error: unable to register topic");
            continue;
        }

        printf("Publishing to topic %s: %s\n", topic.name, states[state]);
        if (emcute_pub(&topic, states[state], strlen(states[state]), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Warning: unable to publish data");
        }

        state = (state + 1) % 2; // Alternate between 0 and 1
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    if (inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6) != 1) {
        puts("Error: unable to parse broker address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    kernel_pid_t pub_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub");
    if (pub_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to start publish thread\n");
        return 1;
    }

    emcute_run(BROKER_PORT, "riot_node");
    return 0;
}