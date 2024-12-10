#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <emcute.h>
#include <xtimer.h>

#define EMCUTE_ID          "device-1"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT        (1883U)
#define MQTT_INTERVAL      (5U) /* seconds */
#define SERVER_IP          "47.102.103.1"

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    /* Convert IP address */
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to convert server address\n");
        return 1;
    }

    /* Start emcute thread */
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Connect to MQTT-SN gateway */
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect [%d]\n", res);
        return 1;
    }

    emcute_topic_t topic;
    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    const char *state_work = "work";
    const char *state_not_work = "not work";

    while (1) {
        unsigned flags = EMCUTE_QOS_0;
        const char *state = (xtimer_now_usec() % 10 < 5) ? state_work : state_not_work;
        if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(MQTT_INTERVAL);
    }

    // Disconnect
    emcute_discon();
    return 0;
}