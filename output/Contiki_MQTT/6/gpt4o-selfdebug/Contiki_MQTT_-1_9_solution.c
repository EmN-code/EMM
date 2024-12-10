#include "mqtt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static char client_id[20];

/* Callback function to handle MQTT events */
static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker at %s\n", MQTT_BROKER_IP);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static struct etimer publish_timer;
    static uint16_t packet_id;
    static char payload[20];

    PROCESS_BEGIN();

    /* Generate client ID */
    snprintf(client_id, sizeof(client_id), "nrf52840-%02x%02x%02x",
             linkaddr_node_addr.u8[5],
             linkaddr_node_addr.u8[6],
             linkaddr_node_addr.u8[7]);

    /* Register the MQTT client */
    mqtt_register(&conn, &mqtt_publisher_process, client_id, mqtt_event_callback, 64);
    mqtt_set_username_password(&conn, "username", "password");

    /* Set the MQTT broker */
    conn.broker_ipaddr.u8[0] = 47;
    conn.broker_ipaddr.u8[1] = 102;
    conn.broker_ipaddr.u8[2] = 103;
    conn.broker_ipaddr.u8[3] = 1;
    conn.broker_port = MQTT_BROKER_PORT;

    /* Connect to the MQTT broker */
    mqtt_connect(&conn);

    /* Set the timer interval */
    etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        /* Create a payload */
        snprintf(payload, sizeof(payload), "work");

        /* Publish the MQTT message */
        mqtt_publish(&conn, &packet_id, MQTT_TOPIC, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        /* Reset the timer */
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}
