
#include "contiki.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "sys/process.h"
#include "sys/clock.h"
#include "string.h"
#include "stdbool.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char state_message[20];

static void mqtt_event_callback(struct mqtt_connection *connection, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static struct etimer et;
    static bool state = true;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, MQTT_CLIENT_ID, mqtt_event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&conn)) {
            if (state) {
                strcpy(state_message, "work");
            } else {
                strcpy(state_message, "not work");
            }
            state = !state;

            mqtt_publish(&conn, NULL, MQTT_TOPIC, (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&mqtt_publisher_process, NULL);
    return 0;
}
