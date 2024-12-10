#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

#include <string.h>  // Include for memset, strlen
#include <stdlib.h>  // Include for bool
#include <ztimer.h>  // Include for ztimer_sleep

#define EMCUTE_PORT     (1883U)
#define TOPIC_NAME      "state"
#define SERVER_IP       "47.102.103.1"

static emcute_topic_t topic;
static sock_udp_ep_t remote;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, EMCUTE_ID);  // Omitted from our blacklist
    return NULL;
}

int main(void) {
    /* initialize thread for emcute */
    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                     EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                                     emcute_thread, NULL, "emcute");

    /* convert IP address */
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = EMCUTE_PORT;

    /* connect to broker */
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%u\n", SERVER_IP, EMCUTE_PORT);
        return 1;
    }

    /* set the message topic */
    strcpy(topic.name, TOPIC_NAME);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    char pub_data[] = "work";
    while (1) {
        if (emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}