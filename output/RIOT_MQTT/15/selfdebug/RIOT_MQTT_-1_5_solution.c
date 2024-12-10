#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/mqtt.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"

static mqtt_client_t client;
static char client_id[] = CLIENT_ID;
static char topic[] = TOPIC;

static void initialize_mqtt(void) {
    mqtt_connect(&client, BROKER_IP, BROKER_PORT, 0, client_id, NULL, NULL, 0, 0);
}

static void publish_state(const char *state) {
    mqtt_publish(&client, topic, (void *)state, strlen(state), MQTT_QOS_0, 0);
    printf("Published: %s\n", state);
}

int main(void) {
    initialize_mqtt();
    const char *state = "work or not work";

    while (1) {
        publish_state(state);
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }

    return 0;
}