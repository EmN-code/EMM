#include <ztimer.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <msg.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <stdatomic.h>
#include <stddef.h>
#include <timex.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <netinet/in.h>
#include <unistd.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("riot-node")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t _topic;

static const char *MQTT_BROKER_IP = "47.102.103.1";
static const char *TOPIC_NAME = "state";

static void *emcute_thread(void *arg) {
    (void)arg;
    static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    /* initialize our thread message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("MQTT-SN connection established\n");
            break;
        }
        else {
            puts("MQTT-SN connection failed, retrying...\n");
            xtimer_sleep(1);
        }
    }
    
    /* setup topic */
    _topic.name = TOPIC_NAME;
    res = emcute_reg(&_topic);
    if (res != EMCUTE_OK) {
        puts("Unable to obtain topic ID\n");
        return NULL;
    }

    while (1) {
        const char *state_msg = "work";

        res = emcute_pub(&_topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            puts("Error: unable to publish data\n");
        } else {
            puts("Published state successfully\n");
        }

        xtimer_sleep(5);
    }

    /* disconnect on finish, though we'll never reach here */
    emcute_discon();
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                   emcute_thread, NULL, "emcute");

    return 0;
}