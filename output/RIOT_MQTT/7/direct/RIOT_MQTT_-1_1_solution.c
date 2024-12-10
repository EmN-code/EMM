#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "mosquitto.h"
#include "xtimer.h"

#define MQTT_HOST "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "riot/esp32/state"

static struct mosquitto *mosq = NULL;

void publish_state(void)
{
    const char *message = "work";
    static int toggle = 0;
    if (toggle) {
        message = "not work";
    }
    toggle = !toggle;

    int ret = mosquitto_publish(mosq, NULL, MQTT_TOPIC, strlen(message), 
                                message, 0, false);
    if (ret) {
        printf("Error publishing to MQTT broker: %s\n", mosquitto_strerror(ret));
    } else {
        printf("Successfully published: %s\n", message);
    }
}

int main(void)
{
    int ret;
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: Could not create Mosquitto client.\n");
        return EXIT_FAILURE;
    }

    ret = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60);
    if (ret) {
        fprintf(stderr, "Unable to connect to MQTT broker: %s\n", mosquitto_strerror(ret));
        return EXIT_FAILURE;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_HOST, MQTT_PORT);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}