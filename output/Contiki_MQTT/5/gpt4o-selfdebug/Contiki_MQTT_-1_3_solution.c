#include "os/net/netstack.h"
#include "os/sys/etimer.h"
#include "os/services/mqtt/mqtt.h"
#include "os/services/mqtt/mqtt-client.h"

PROCESS(mqtt_example, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example);

static struct etimer timer;
static mqtt_client_t client;

// Callback function for the MQTT client
static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to the broker\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from the broker\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_example, ev, data) {
    PROCESS_BEGIN();

    // Set the broker address
    mqtt_register( &client, &mqtt_example, "mqtt_client", mqtt_event, 512);
    mqtt_connect(&client, "47.102.103.1", 1883, 60);

    etimer_set(&timer, CLOCK_SECOND * 5);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Publish message
        char *msg = "work"; // Change this logic as needed
        mqtt_publish(&client, NULL, "device/state", (uint8_t *)msg, strlen(msg), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
  PROCESS_BEGIN();
  process_start(&mqtt_example, NULL);
  PROCESS_END();
}