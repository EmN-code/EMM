#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/mqtt.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define PUBLISH_INTERVAL  (5U * US_PER_SEC)

static char mqtt_client_id[64];
static char publish_message[64];
static mqtt_client_t mqtt_client;
static sock_udp_ep_t broker_endpoint;

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message) {
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    msg.qos = 0;  // QoS 0
    msg.payload = (void *)message;
    msg.payloadsize = strlen(message);

    if (mqtt_publish(client, topic, &msg) != MQTT_OK) {
        printf("Error: unable to publish message\n");
    }
}

int main(void) {
    memset(&broker_endpoint, 0, sizeof(broker_endpoint));
    broker_endpoint.family = AF_INET;
    broker_endpoint.port = MQTT_BROKER_PORT;
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker_endpoint.addr.ipv4) != 1) {
        printf("Error: bad IP address\n");
        return -1;
    }

    snprintf(mqtt_client_id, sizeof(mqtt_client_id), "esp32_client_%i", rand());

    if (mqtt_init(&mqtt_client, NULL, mqtt_client_id, NULL, 0, false) != MQTT_OK) {
        printf("Error: unable to initialize MQTT client\n");
        return -1;
    }

    if (mqtt_connect(&mqtt_client, "", 0, false, 0) != MQTT_OK) {
        printf("Error: unable to connect to MQTT broker\n");
        return -1;
    }

    printf("MQTT client connected\n");

    strcpy(publish_message, "Device state: work");
    while (1) {
        mqtt_publish(&mqtt_client, "device/state", publish_message);
        printf("Published: %s\n", publish_message);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}