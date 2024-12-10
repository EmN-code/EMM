#include "mma8x5x.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "mutex.h"
#include "evtimer_mbox.h"
#include "stdatomic.h"
#include "stdio.h"
#include "mbox.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "dht.h"
#include "stddef.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "timex.h"
#include "mma8x5x_params.h"

#define EMCUTE_ID       1
#define EMCUTE_PORT     1883
#define MQ_TOPIC_NAME   "state"
#define MQ_MSG_WORK     "work"
#define MQ_MSG_NOT_WORK "not work"
#define MQ_QOS          0

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t emcute_queue[8];
static emcute_topic_t topic;

static const sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

static int publish_state(uint8_t is_working)
{
    const char *message = is_working ? MQ_MSG_WORK : MQ_MSG_NOT_WORK;
    return emcute_pub(&topic, message, strlen(message) + 1, MQ_QOS);
}

static int connect_to_mqtt(void)
{
    int res;
    while (1) {
        res = emcute_con((sock_udp_ep_t *) &remote, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to broker\n");
            break;
        }
        else {
            printf("Failed to connect, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }
    topic.name = MQ_TOPIC_NAME;
    return emcute_reg(&topic);
}

int main(void)
{
    puts("MQTT-SN example for RIOT");
    emcute_run(emcute_stack, sizeof(emcute_stack), EMCUTE_ID, "mqtt-sample");
    emcute_sock_udp_ep_init((sock_udp_ep_t*) &remote, remote.port, remote.family);

    if (connect_to_mqtt() != EMCUTE_OK) {
        puts("Error: unable to connect/register to MQTT-SN broker\n");
        return 1;
    }

    while (1) {
        uint8_t is_working = 1; // Here you can decide dynamically which state to publish
        if (publish_state(is_working) != EMCUTE_OK) {
            puts("Error: unable to publish message\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}