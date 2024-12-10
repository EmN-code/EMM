#include "ztimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "mbox.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "msg.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "timex.h"

#define EMCUTE_PRIO          (THREAD_PRIORITY_MAIN - 1)
#define MAIN_QUEUE_SIZE      (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote_gateway = { .family = AF_INET6, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &remote_gateway.addr.ipv6);
    
    if (emcute_con(&remote_gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        // Handle connection error
    }

    emcute_topic_t topic;
    topic.name = "state";
    
    if (emcute_reg(&topic) != EMCUTE_OK) {
        // Handle topic registration error
    }

    char pub_data[] = "work or not work";
    while (1) {
        if (emcute_pub(&topic, pub_data, sizeof(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            // Handle publish error
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt");
    
    return 0;
}