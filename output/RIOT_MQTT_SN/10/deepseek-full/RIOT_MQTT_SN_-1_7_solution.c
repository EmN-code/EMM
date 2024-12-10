
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <msg.h>
#include <mbox.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <ztimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000 * 1000) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void publish_state(void)
{
    static bool state = true;
    const char *msg = state ? MSG_WORK : MSG_NOT_WORK;
    state = !state;

    int res = emcute_pub(&topic, msg, strlen(msg), 0);
    if (res != EMCUTE_OK) {
        puts("Error publishing message");
    }
}

int main(void)
{
    /* Initialize the network stack */
    if (esp_wifi_init() != ESP_OK) {
        puts("Error initializing WiFi");
        return 1;
    }

    /* Connect to the broker */
    if (inet_aton(BROKER_IP, &gw.addr.ipv4) == 0) {
        puts("Error parsing broker IP address");
        return 1;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    /* Register the topic */
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
