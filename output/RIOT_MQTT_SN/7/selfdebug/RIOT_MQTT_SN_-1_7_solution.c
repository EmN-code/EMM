#include <xtimer.h>
#include <net/emcute.h>
#include <net/conn/ip.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "riot-esp32"
#define TOPIC_NAME "device/state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char pub_buf[256];

static void publish_state(void) {
    const char *state = "work";
    size_t len = strlen(state);
    memcpy(pub_buf, state, len);
    topic.name = TOPIC_NAME;
    topic.id = 0; // Assume the topic is already registered
    int res = emcute_pub(&topic, pub_buf, len, EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data (%d)\n", res);
    }
}

int main(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse broker IP address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the broker\n");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    emcute_discon();
    return 0;
}