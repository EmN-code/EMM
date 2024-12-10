#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[NUMOFSUBS];

static emcute_topic_t topic;
static char *server_ip = "47.102.103.1";

static void *emcute_thread(void *arg) {
    (void)arg;

    ztimer_sleep(ZTIMER_MSEC, 5000);
    
    while (1) {
        unsigned flags = EMCUTE_QOS_0;
        char *state_message = "work";
        
        if (emcute_pub(&topic, state_message, strlen(state_message), flags) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, server_ip, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the gateway\n");
        return 1;
    }

    strcpy(topic.name, "sensor_state");
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    emcute_thread(NULL);

    return 0;
}