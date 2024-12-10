#include "simple-udp.h"
#include "mqtt.h"
#include "contiki.h"

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char *state_topic = "device/status";
static char state_msg[15];

PROCESS_THREAD(mqtt_publisher_process, ev, data){
    static struct etimer timer;
    static uint16_t mid;

    PROCESS_BEGIN();

    // Register the MQTT connection
    mqtt_register(&conn, &mqtt_publisher_process, "nrf52840_device", NULL, MQTT_MAX_SEGMENT_SIZE);

    // Set up the timer to trigger every five seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1){
        // Connect to the MQTT broker
        mqtt_connect(&conn, "47.102.103.1", 1883, 60);
        etimer_reset(&timer);

        while(mqtt_ready(&conn) == 0) {
            etimer_reset(&timer);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Prepare state message
        sprintf(state_msg, "work");

        // Publish the state message
        mqtt_publish(&conn, &mid, state_topic, (uint8_t *)state_msg, strlen(state_msg), MQTT_QOS_LEVEL_0);
    }

    PROCESS_END();
}