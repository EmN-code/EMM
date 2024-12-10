#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <esp_system.h>
#include <event/thread.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <net/gcoap.h>

#define EMCUTE_PORT     (1883U)
#define MQTT_SN_ID      "esp32client"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t my_topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

void *publisher(void *arg) {
    (void)arg;
    const char *state_message = "work or not work";
    while (1) {
        if (emcute_pub(&my_topic, state_message, strlen(state_message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        sleep(5);
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    my_topic.name = "state";
    if (emcute_reg(&my_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher, NULL, "publisher");

    emcute_run(EMCUTE_PORT, MQTT_SN_ID);
    return 0;
}