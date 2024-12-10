#include <string.h>
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nameserver.h"
#include "mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "topic/state"

static struct mqtt_connection conn;
static char *state_msg = "work";

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    static struct etimer timer;
    static uint16_t packet_id = 0;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, CLIENT_ID, mqtt_event, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUBLISH_INTERVAL * 3);

    etimer_set(&timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER && data == &timer) {
            mqtt_status_t status = mqtt_publish(&conn, 
                                                &packet_id, 
                                                MQTT_TOPIC, 
                                                (uint8_t *)state_msg, 
                                                strlen(state_msg), 
                                                MQTT_QOS_LEVEL_0);
            if(status != MQTT_STATUS_OK) {
                printf("Failed to publish message
");
            }
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}

static void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connection established
");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT connection terminated
");
    }
}