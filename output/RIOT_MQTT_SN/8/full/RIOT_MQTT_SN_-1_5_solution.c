#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "stddef.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL 5000

static emcute_sub_t subscription;
static char state_message[] = "work";

void *publish_thread(void *arg) {
    emcute_topic_t topic;
    topic.name = "device/state";
    int res = emcute_reg(&topic);

    if (res != EMCUTE_OK) {
        printf("Topic registration failed: %d\n", res);
        return NULL;
    }

    while (1) {
        res = emcute_pub(&topic, state_message, sizeof(state_message), 0);

        if (res != EMCUTE_OK) {
            printf("Publish failed: %d\n", res);
        }
        usleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    char publish_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(publish_stack, sizeof(publish_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, publish_thread,
                  NULL, "publish_thread");

    while (1) {
        int res = emcute_con(&remote, true, NULL, NULL, 0, 0);

        if (res != EMCUTE_OK) {
            printf("Connection failed: %d\n", res);
            sleep(1);
            continue;
        }
        printf("Connected successfully.\n");
        break;
    }

    pause(); // Keep the main thread running

    return 0;
}