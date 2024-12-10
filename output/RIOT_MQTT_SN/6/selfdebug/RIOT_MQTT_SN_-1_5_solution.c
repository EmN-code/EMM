#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define EMCUTE_ID "esp32_client"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

enum {
    STATE_WORK = 0,
    STATE_NOT_WORK,
};

static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static emcute_topic_t topic = { "device/state", 0 };

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(SERVER_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    puts("Starting MQTT-SN client on ESP32\n");

    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("ERROR: Unable to parse broker IP address\n");
        return 1;
    }

    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    while (1) {
        static int state = STATE_WORK;
        const char *state_str = (state == STATE_WORK) ? "work" : "not work";
        if (emcute_pub(&topic, state_str, strlen(state_str), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("ERROR: unable to publish data\n");
        }
        state = (state == STATE_WORK) ? STATE_NOT_WORK : STATE_WORK;
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}