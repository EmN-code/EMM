#include <xtimer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BUFFER_SIZE         (128U)
#define TOPIC_NAME          "device/status"
#define MQTT_SN_ADDR        "47.102.103.1"
#define MQTT_SN_PORT        (1883U)
#define MQTT_SN_INTERVAL    (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topics[1];
static char buffer[BUFFER_SIZE];
static sock_udp_ep_t gw;

static void *emcute_thread(void *arg) {
    (void)arg;
    while (true) {
        /* toggle state */
        static bool state = false;
        state = !state;

        /* prepare the MQTT-SN message */
        snprintf(buffer, sizeof(buffer), "Device is %s", state ? "working" : "not working");

        /* publish the message */
        emcute_pub(&topics[0], buffer, strlen(buffer), EMCUTE_QOS_0);
        printf("Published: %s\n", buffer);

        /* sleep to save energy */
        xtimer_sleep(MQTT_SN_INTERVAL);
    }
    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    /* handle the incoming publish message here */
}

int main(void) {
    /* initialize the communication */
    emcute_init();

    /* set IP and port of the MQTT-SN broker */
    gw.family = AF_INET;
    inet_pton(AF_INET, MQTT_SN_ADDR, &gw.addr.ipv4);
    gw.port = MQTT_SN_PORT;

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to MQTT-SN broker\n");
        return 1;
    }
    puts("Connected to MQTT-SN broker\n");

    /* set the topic we want to work with */
    topics[0].name = TOPIC_NAME;
    emcute_reg(&topics[0]);

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    return 0;
}