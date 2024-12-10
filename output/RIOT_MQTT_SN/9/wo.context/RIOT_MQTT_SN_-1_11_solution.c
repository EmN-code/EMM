#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/af.h>

#define EMCUTE_PORT          (1883U)
#define EMCUTE_ID            ("esp32_client")
#define MQTT_BROKER_IP       "47.102.103.1"
#define PUBLISH_INTERVAL     (5U * US_PER_SEC) // 5 seconds

static emcute_topic_t topic;
static char *state_message = "work or not work";

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) == 0) {
        printf("Error: unable to parse remote address\n");
        return -1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway at [%s]:%d\n", MQTT_BROKER_IP, EMCUTE_PORT);
        return -1;
    }

    printf("Connected to [%s]:%d\n", MQTT_BROKER_IP, EMCUTE_PORT);
    return 0;
}

static int publish_state(void) {
    unsigned flags = EMCUTE_QOS_0;

    if (emcute_pub(&topic, state_message, strlen(state_message), flags) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
        return -1;
    }

    printf("Published: %s\n", state_message);
    return 0;
}

int main(void) {
    /* Initialize emcute for the ESP32 */
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    /* Connect to the MQTT-SN broker */
    if (connect_to_broker() != 0) {
        return 1;
    }

    /* We assume the topic is registered and assigned an ID previously */
    topic.name = "state";
    topic.id = 1; // Example topic ID, ensure it's valid and registered

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}