#include "mqtt.h"
#include "contiki.h"
#include "dev/leds.h"

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct etimer timer;
static struct mqtt_connection conn;

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED: 
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED: 
            printf("MQTT disconnected\n");
            break;
        default:
            printf("MQTT event %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&timer, 5 * CLOCK_SECOND);

    mqtt_register(&conn, &mqtt_example_process, "nrf52840_client", mqtt_event, 64);
    mqtt_connect(&conn, "47.102.103.1", 1883, 60);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        uint16_t mid;
        char *topic = "device/state";
        char *message = "work";
        mqtt_publish(&conn, &mid, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0);

        etimer_reset(&timer);
        leds_toggle(LEDS_RED);
    }

    PROCESS_END();
}