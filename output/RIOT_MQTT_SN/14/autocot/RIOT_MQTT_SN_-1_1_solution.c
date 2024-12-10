#include <string.h>
#include "shell.h"
#include "msg.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define STATE_TOPIC "device/state"
#define RIOT_MQTT_CLIENT_ID "riot_esp32_client"
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EMCUTE_PORT (1883U)

static emcute_topic_t topics[1];
static char stack[STACKSIZE];
static msg_t queue[8];
static const char *state_working = "work";
static const char *state_not_working = "not work";
static bool is_working = true;

static void *pub_thread(void *arg) {
    (void)arg;
    while (1) { 
        const char *state = is_working ? state_working : state_not_working;
        emcute_topic_t *t = &topics[0];

        if (emcute_pub(t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        is_working = !is_working;
        xtimer_sleep(5);
    }

    return NULL;
}

static int connect_mqtt(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

    if (inet_pton(AF_INET6, SERVER_IP, &gw.addr.ipv6) == 0) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }

    if (emcute_con(&gw, true, RIOT_MQTT_CLIENT_ID, NULL, NULL, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");
    return 0;
}

int main(void) {
    /* set the default backend to UDP */
    emcute_sock_udp_ep_t ep;
    ep.port = EMCUTE_PORT;
    ep.family = AF_INET6;

    msg_init_queue(queue, 8);
    emcute_run(stack, sizeof(stack), EMCUTE_PORT, &ep);

    /* setup the MQTT topics */
    topics[0].name = STATE_TOPIC;
    topics[0].id = 0;

    /* connect to MQTT broker */
    if (connect_mqtt() != 0) {
        return 1;
    }

    /* start publication thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    return 0;
}