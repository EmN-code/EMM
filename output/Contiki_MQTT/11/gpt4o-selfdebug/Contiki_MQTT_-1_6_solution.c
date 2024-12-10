#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-icmp6.h"
#include <stdio.h>
#include"contiki.h"

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer timer;
static struct mqtt_connection conn;
static char client_id[] = "nrf52840_publisher";
static char pub_topic[] = "device/state";
static char payload[] = "work";
static mqtt_status_t status;
static uint16_t mid;

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize MQTT connection
    status = mqtt_register(&conn, &mqtt_publisher_process, client_id, NULL, 256);
    if (status != MQTT_STATUS_OK) {
        printf("MQTT register failed: %d\n", status);
        PROCESS_EXIT();
    }

    printf("MQTT registered successfully\n");

    // Connect to the MQTT broker
    static mqtt_connect_status_t connect_status;
    connect_status = mqtt_connect(&conn, "47.102.103.1", 1883, 100, MQTT_KEEP_ALIVE_DEFAULT);
    if (connect_status != MQTT_CONNECT_ACK_ACCEPTED) {
        printf("MQTT connect failed: %d\n", connect_status);
        PROCESS_EXIT();
    }

    printf("Connected to MQTT broker\n");

    // Set a timer to publish every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();

        if (etimer_expired(&timer)) {
            // Publish message
            status = mqtt_publish(&conn, &mid, pub_topic, (uint8_t *)payload, sizeof(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_NONE, NULL);
            if (status != MQTT_STATUS_OK) {
                printf("MQTT publish failed: %d\n", status);
            } else {
                printf("Published to topic %s\n", pub_topic);
            }
            
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}