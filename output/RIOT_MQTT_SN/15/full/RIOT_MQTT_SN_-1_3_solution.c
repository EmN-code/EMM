#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID_LEN       (64U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    if (sock_udp_ep_from_str(&gw, "47.102.103.1:1883") < 0) {
        printf("Error: unable to parse address\n");
        return NULL;
    }

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
            printf("Error: connection to gateway failed\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }

        topic.name = "device/state";
        res = emcute_reg(&topic);
        if (res != EMCUTE_OK) {
            printf("Error: unable to register topic\n");
            emcute_discon();
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }

        while (1) {
            const char *message = "work";
            res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
            if (res != EMCUTE_OK) {
                printf("Error: unable to publish\n");
            }
            ztimer_sleep(ZTIMER_SEC, 5);
        }
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT-SN example\n");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqtt");
    return 0;
}