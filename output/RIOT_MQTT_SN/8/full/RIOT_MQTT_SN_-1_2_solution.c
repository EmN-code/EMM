#include "net/ipv6/addr.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "thread.h"
#include "stdio.h"
#include "mma8x5x_regs.h"

#define EMCUTE_PORT      (1883U)
#define SERVER_ADDR      "47.102.103.1"
#define TOPIC_NAME       "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET,
                            .port = EMCUTE_PORT };

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *msg = (char *)data;
    printf("Received publication on topic %s: %s\n", topic->name, msg);
}

static void *emcute_thread(void *arg) {
    (void)arg;
    char *message = "work";

    while (1) {
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to publish data\n");
        } else {
            printf("Published data successfully: %s\n", message);
        }
        sleep(5);
    }

    return NULL;
}

static void connect_to_gateway(void) {
    printf("[MQTT] Connecting to GW at [%s]:%d\n", SERVER_ADDR, EMCUTE_PORT);

    while (1) {
        int result = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (result == EMCUTE_OK) {
            printf("Connection to gateway successful!\n");
            break;
        } else {
            printf("Connection failed, retrying...\n");
        }
        sleep(1);
    }

    printf("[MQTT] Registering topic \"%s\"\n", TOPIC_NAME);
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Topic registration failed\n");
    }
}

int main(void) {
    gw.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR);

    connect_to_gateway();

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_SLEEPING, emcute_thread, NULL, "emcute_thread");

    thread_wakeup(pid);

    while (1) {
        thread_sleep();
    }

    return 0;
}