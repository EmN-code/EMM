#include "sys/process.h"
#include "mqtt_iface.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work or not work"
#define PUBLISH_INTERVAL 5  // in seconds

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static void mqtt_handle_event(mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            process_exit(&mqtt_example_process);
            break;
        case MQTT_EVENT_SUBACK:
        case MQTT_EVENT_PUBACK:
        case MQTT_EVENT_UNSUBACK:
        default:
            break;
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    static struct etimer timer;
    static struct mqtt_connection my_mqtt_conn;
    static uint16_t msg_id;
    PROCESS_BEGIN();

    mqtt_register(&my_mqtt_conn, &mqtt_example_process, "mqtt-client-id-001", mqtt_handle_event, 128);
    mqtt_connect(&my_mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, (60 * CLOCK_SECOND) >> 1);

    etimer_set(&timer, CLOCK_SECOND * PUBLISH_INTERVAL);
    while(1) {
        PROCESS_YIELD();
        if(etimer_expired(&timer)) {
            mqtt_publish(&my_mqtt_conn, &msg_id, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}
