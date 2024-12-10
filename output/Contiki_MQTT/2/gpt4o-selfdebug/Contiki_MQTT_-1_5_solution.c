#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/psock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt.h"
#include "contiki.h"

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static char client_id[] = "nrf52840_client";
static char topic[] = "state";
static char payload[] = "work or not work";

static mqtt_status_t mqtt_connect_and_publish(void) {
    mqtt_status_t status;
    
    // Establish MQTT connection
    status = mqtt_register(&conn, &mqtt_example_process, client_id, NULL, 64);
    if(status != MQTT_STATUS_OK) {
        printf("Failed to register MQTT connection\n");
        return status;
    }

    status = mqtt_connect(&conn, "47.102.103.1", 1883, 60);
    if(status != MQTT_STATUS_OK) {
        printf("Failed to connect to MQTT broker\n");
        return status;
    }

    uint16_t mid;
    status = mqtt_publish(&conn, &mid, topic, (void *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
    if(status != MQTT_STATUS_OK) {
        printf("Failed to publish message\n");
        return status;
    }

    printf("Message published with mid %u\n", mid);
    return status;
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, 5 * CLOCK_SECOND);
        
        // Wait for timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        mqtt_status_t status = mqtt_connect_and_publish();
        if(status == MQTT_STATUS_OK) {
            printf("Successfully published state\n");
        } else {
            printf("Error in publishing\n");
        }
    }

    PROCESS_END();
}