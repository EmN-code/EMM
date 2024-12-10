#include <stdio.h>
#include <string.h>
#include <mosquitto.h>

#define MQTT_HOST "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "RIOT_ESP32"

static bool work_state = true;

void publish_state(struct mosquitto *mosq) {
    char message[20];
    snprintf(message, sizeof(message), "state: %s", work_state ? "work" : "not work");
    mosquitto_publish(mosq, NULL, MQTT_TOPIC, strlen(message), message, 0, false);
    printf("Published: %s\n", message);
}

int main(void) {
    struct mosquitto *mosq;

    // Initialize the Mosquitto library
    mosquitto_lib_init();

    // Create a new Mosquitto client instance
    mosq = mosquitto_new(MQTT_CLIENT_ID, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Failed to create Mosquitto client\n");
        return -1;
    }

    // Connect to the specified MQTT broker
    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Unable to connect to MQTT broker\n");
        return -1;
    }

    // Loop to publish the device state every 5 seconds
    while (1) {
        publish_state(mosq);
        sleep(5);
    }

    // Clean up Mosquitto library
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}