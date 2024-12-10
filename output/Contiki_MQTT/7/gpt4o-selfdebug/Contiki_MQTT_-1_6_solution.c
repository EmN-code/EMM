#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Including MQTT and UDP headers
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6.h"

// Assuming MQTT library header
#include "mqtt.h"

// Definitions for the MQTT broker
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_client_process, "MQTT Client");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, "client_id",
                  NULL, 256);

    mqtt_set_username_password(&conn, "username", "password");

    etimer_set(&periodic_timer, PUB_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        printf("Connecting to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);
        mqtt_status_t status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT,
                                            60, 0, 0, 0, NULL);

        if(status != MQTT_STATUS_OK) {
            printf("Failed to connect to the broker\n");
            continue;
        }

        const char *topic = "status";
        const char *message = "work";
        printf("Publishing to topic '%s': %s\n", topic, message);

        if(mqtt_publish(&conn, NULL, topic, (uint8_t *)message, strlen(message),
                        MQTT_QOS_LEVEL_0, 0) != MQTT_STATUS_OK) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published successfully\n");
        }

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&mqtt_client_process, NULL);
    while (1) {
      ; // Main intentionally left in infinite loop to continue process scheduling
    }
}