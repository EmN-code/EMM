#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "thread.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t thread_pid;
static char thread_stack[STACKSIZE];

void *publisher_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    sock_udp_name2ep(&gw, BROKER_IP);

    while (1) {
        char *message = "work";

        topic.name = TOPIC_NAME;

        int res = emcute_pub(&topic, message, strlen(message), 0);
        if (res != EMCUTE_OK) {
            printf("Could not publish message [%i]\n", res);
        } else {
            printf("Published: %s\n", message);
        }

        usleep(5 * 1000000); // sleep 5 seconds
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    sock_udp_name2ep(&gw, BROKER_IP);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Could not connect to gateway [%i]\n", res);
        return 1;
    }

    thread_pid = thread_create(thread_stack, STACKSIZE, THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, publisher_thread,
                               NULL, "publisher");

    return 0;
}