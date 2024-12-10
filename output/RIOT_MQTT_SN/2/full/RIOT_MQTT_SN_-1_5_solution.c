#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdatomic.h"
#include "dht_params.h"
#include "thread.h"
#include "net/gcoap.h"
#include "xtimer.h"
#include "net/ipv6/addr.h"
#include "stdio.h"
#include "stdbool.h"
#include "timex.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "string.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "dht.h"
#include "assert.h"
#include "ztimer.h"
#include "stddef.h"
#include "mutex.h"

#define EMCUTE_PORT      (1883U)
#define SERVER_PORT     "1883"
#define SERVER_ADDR     "47.102.103.1"
#define TOPIC_NAME      "status"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char topname[64U];

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

    memcpy(gw.addr.ipv6, SERVER_ADDR, sizeof(gw.addr.ipv6));

    while(1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);

        if (res != EMCUTE_OK) {
            printf("Connection refused (code %d)\n", res);
            xtimer_sleep(5);
            continue;
        }

        memset(topname, 0, sizeof(topname));
        strcpy(topname, TOPIC_NAME);

        topic.name = topname;

        if (emcute_reg(&topic) != EMCUTE_OK) {
            printf("Error: unable to register topic\n");
            emcute_discon();
            xtimer_sleep(5);
            continue;
        }

        char *state = "work";

        while (1) {
            printf("Publishing state: %s\n", state);

            if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
                printf("Error: unable to publish data\n");
                xtimer_sleep(5);
                break;
            }

            state = (strcmp(state, "work") == 0) ? "not work" : "work";
            xtimer_sleep(5);
        }

        emcute_discon();
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}