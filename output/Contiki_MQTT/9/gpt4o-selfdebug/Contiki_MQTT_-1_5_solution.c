#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include <stdio.h>

#define MQTT_TOPIC "nrf52840/status"
#define MQTT_MSG "work"
#define MQTT_BROKER_IP "47.102.103.1"

static struct mqtt_connection conn;
static char client_id[] = "nrf52840";

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize MQTT connection
    mqtt_register(&conn, &mqtt_publisher_process, client_id, NULL, 128);

    mqtt_connect(&conn, MQTT_BROKER_IP, 1883, 60, 1, NULL);

    // Set a timer for periodic publishing
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&timer)) {
            const char *msg = MQTT_MSG;
            uint16_t mid;
            mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)msg, strlen(msg), 0, 0, 0, NULL);
            printf("Published message: %s\n", msg);
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}

int main() {
    return 0;
}