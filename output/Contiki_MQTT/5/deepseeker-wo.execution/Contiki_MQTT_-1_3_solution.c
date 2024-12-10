
#include "contiki.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "dev/button-hal.h"
#include "dht11-sensor.h"
#include "coap-blocking-api.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

static struct mqtt_connection conn;
static char message[] = "work";

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static struct etimer et;
    static uint16_t mid;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, "nrf52840_client", mqtt_event_callback, 128);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)message, sizeof(message) - 1, MQTT_QOS_LEVEL_0, 0, 0, MQTT_TOPIC_ALIAS_EN_NONE, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&mqtt_publisher_process, NULL);
    return 0;
}
