
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static volatile bool state = true; // true for "work", false for "not work"

static void _publish_state(void)
{
    emcute_topic_t topic;
    char msg[10];

    if (state) {
        strcpy(msg, MSG_WORK);
    } else {
        strcpy(msg, MSG_NOT_WORK);
    }

    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Message published successfully");
    }

    state = !state; // Toggle state
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str((uint8_t *)&gw.addr.ipv4, MQTT_BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");

    while (1) {
        _publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
