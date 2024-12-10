#include "xtimer.h"
#include "net/nimble.h"
#include "mqtt.h"
#include "net/emcute.h"

#define MQTT_SERVER_ADDRESS "47.102.103.1"
#define MQTT_PORT 1883
#define STATE_TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static mqtt_client_t client;
static mqtt_message_t message;

void mqtt_publish_state(void)
{
    static bool is_working = true;
    char payload[20];

    snprintf(payload, sizeof(payload), "state: %s", is_working ? "work" : "not work");
    is_working = !is_working;  // Toggle state for next publish

    message.payload_data = payload;
    message.payload_length = strlen(payload);

    mqtt_publish(&client, STATE_TOPIC, &message);
}

int main(void)
{
    printf("Starting MQTT state publisher\n");

    mqtt_initialize(&client, MQTT_SERVER_ADDRESS, MQTT_PORT, 0, NULL, NULL, NULL);
    mqtt_connect(&client);

    message.qos = MQTT_QOS_1;

    while (1)
    {
        mqtt_publish_state();
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}