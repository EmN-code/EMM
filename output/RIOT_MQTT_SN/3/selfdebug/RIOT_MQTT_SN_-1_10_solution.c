#include <xtimer.h>
#include <string.h>
#include <sock.h>
#include <net/ipv6/addr.h>

#include "msg.h"
#include "shell.h"
#include "thread.h"
#include "emcute.h"

#define MAIN_QUEUE_SIZE     (8)
#define EMCUTE_ID           (1)
#define EMCUTE_PORT         (1883)
#define SERVER_ADDR         "47.102.103.1"
#define PUB_INTERVAL_SEC    (5)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static emcute_topic_t topic;

static void *mqttsn_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
        printf("Error with IPv6 address format for gateway!\n");
        return NULL;
    }
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Unable to connect to the gateway...\n");
        return NULL;
    }

    topic.name = "status";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Unable to obtain topic ID\n");
        return NULL;
    }

    while (1) {
        const char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: Unable to publish message\n");
        } else {
            printf("Published: %s\n", message);
        }
        xtimer_sleep(PUB_INTERVAL_SEC);
    }
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqttsn_thread, NULL, "MQTTSN thread");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}