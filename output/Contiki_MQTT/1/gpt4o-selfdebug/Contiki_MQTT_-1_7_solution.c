#include <stdio.h>
#include <string.h>
#include <sys/process.h>
#include <mqtt/mqtt.h>

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static char client_id[] = "nrf52840";
static char topic[] = "device/state";

static void publish_state(void) {
    uint16_t msg_id;
    char payload[] = "work or not work";
    mqtt_publish(&conn, &msg_id, topic, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0);
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();
    
    mqtt_register(&conn, &mqtt_example_process, client_id, NULL, 1000);
    mqtt_connect(&conn, "47.102.103.1", 1883, 60, 1, NULL);

    while (1) {
        publish_state();
        PROCESS_PAUSE();
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    }

    PROCESS_END();
}