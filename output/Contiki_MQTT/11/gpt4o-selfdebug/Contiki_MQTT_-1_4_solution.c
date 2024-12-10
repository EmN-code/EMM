#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-udp-packet.h"
#include <mqtt.h>
#include <stdio.h>
#include "contiki.h"
#include "sys/etimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static struct etimer publish_timer;
static char client_id[] = "nrf52840_device";
static char pub_topic[] = "device/state";
static char payload[] = "work"; // Payload can be changed as needed
static mqtt_status_t status;
static struct mqtt_connection conn;

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

PROCESS_THREAD(nrf52840_mqtt_process, ev, data)
{
    PROCESS_BEGIN();
    
    etimer_set(&publish_timer, CLOCK_SECOND * PUBLISH_INTERVAL);

    printf("Connecting to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    
    status = mqtt_register(&conn, &nrf52840_mqtt_process, client_id, NULL, 64);
    if(status != MQTT_STATUS_OK) {
        printf("Failed to register MQTT connection.\n");
        PROCESS_EXIT();
    }

    status = mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 1000, MQTT_CLEAN_SESSION_ON, NULL);
    if(status != MQTT_STATUS_OK) {
        printf("Failed to connect to the MQTT broker.\n");
        PROCESS_EXIT();
    }

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        printf("Publishing state to topic %s\n", pub_topic);
        mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, NULL);

        etimer_reset(&publish_timer);
    }

    mqtt_disconnect(&conn);
    PROCESS_END();
}