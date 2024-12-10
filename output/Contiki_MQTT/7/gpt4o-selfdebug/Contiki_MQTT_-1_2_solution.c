#include <stdio.h>
#include <mqtt.h>
#include <contiki.h>
#include <sys/process.h>

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static struct mqtt_connection conn;
static struct etimer timer;

const char *broker_ip = "47.102.103.1";
#define BROKER_PORT 1883
const char *pub_topic = "nrf52840/state";
char *pub_message = "work";

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    }
    else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publish_process, "nrf52840-client", mqtt_event_callback, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, broker_ip, BROKER_PORT, (30 * CLOCK_SECOND), MQTT_CLEAN_SESSION_ON);

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        static uint16_t msg_id;
        mqtt_publish(&conn, &msg_id, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        printf("Message Published: %s\n", pub_message);
    }

    PROCESS_END();
}