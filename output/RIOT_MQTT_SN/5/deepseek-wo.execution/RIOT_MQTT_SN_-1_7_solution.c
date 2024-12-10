
#include <stdlib.h>
#include <net/gcoap.h>
#include <dht.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <mma8x5x.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_regs.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static emcute_topic_t topic;

void publish_state(void) {
    while (1) {
        int res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing message: %d\n", res);
        } else {
            printf("Message published successfully\n");
        }
        thread_sleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)publish_state, NULL, "publisher");

    return 0;
}
