
#include <stdio.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>

#define TOPIC "state"
#define MSG "work"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;

static void _register_topic(void) {
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }
    printf("Registered topic: %s with ID: %u\n", topic.name, (unsigned)topic.id);
}

static void _publish_state(void) {
    const char *msg = (rand() % 2 == 0) ? "work" : "not work";
    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
        return;
    }
    printf("Published: %s\n", msg);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }
    puts("Connected to MQTT-SN gateway");

    _register_topic();

    while (1) {
        _publish_state();
        xtimer_usleep(INTERVAL);
    }

    emcute_discon();
    return 0;
}
