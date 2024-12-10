#include <ztimer.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       "esp32_node"
#define EMCUTE_TOPIC_MAXLEN 64
#define EMCUTE_BUFSIZE  128

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     1883
#define STATE_TOPIC     "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static char emcute_stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t state_topic;

void on_event(void *arg)
{
    (void)arg; // Unused

    mqtt_client_t *client = (mqtt_client_t *)arg;

    char payload[] = "work";
    size_t payload_len = sizeof(payload);

    int res = emcute_pub(&state_topic, payload, payload_len, EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish data [%d]\n", res);
    }
}

static void *thread_pub(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    state_topic.name = STATE_TOPIC;

    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        on_event(NULL);
        ztimer_sleep(ZTIMER_MSEC, 5000); // sleep for 5 seconds
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_run,
                  NULL, "emcute");

    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_pub,
                  NULL, "pub_thread");

    return 0;
}