#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>      /* For memcpy */
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <netinet/in.h>  /* For struct sockaddr_in */
#include <arpa/inet.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_BUFSIZE      (128U)
#define EMCUTE_PORT         (1883U)
#define SERVER_IP           "47.102.103.1"
#define TOPIC_NAME          "state"
#define INTERVAL            (5U) /* in seconds */

extern void *mqtt_handler(void *arg);

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void *mqtt_handler(void *arg) {
    (void) arg;
    sock_udp_ep_t remote;
    remote.port = EMCUTE_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to convert IP address.\n");
        return NULL;
    }
    
    remote.family = AF_INET;

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to MQTT-SN gateway.\n");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic.\n");
        return NULL;
    }

    char *state_msg = "work";
    while (1) {
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: Unable to publish message.\n");
        }
        ztimer_sleep(ZTIMER_MSEC, INTERVAL * 1000);
    }
    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    /* Start MQTT-SN handler thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_handler, NULL, "mqtt_handler");
    
    /* Keep the main thread alive */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL * 1000);
    }
    return 0;
}