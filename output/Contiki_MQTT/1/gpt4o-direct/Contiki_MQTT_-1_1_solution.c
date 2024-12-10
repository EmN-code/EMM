#include <contiki.h>
#include <mqtt.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct etimer periodic_timer;
static char client_id[32];
static struct mqtt_connection conn;

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to the MQTT broker\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from the MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_client_process, ev, data) {
    static char message[20];
    static int work_state = 1;

    PROCESS_BEGIN();

    snprintf(client_id, sizeof(client_id), "nrf52840-client");
    mqtt_register(&conn, &mqtt_client_process, client_id, mqtt_event, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, (60 * CLOCK_SECOND), MQTT_CLEAN_SESSION_ON);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&periodic_timer)) {
            if (work_state) {
                strcpy(message, "work");
            } else {
                strcpy(message, "not work");
            }
            mqtt_publish(&conn, NULL, "state", (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            work_state = !work_state; // Toggle working state for demonstration
            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}