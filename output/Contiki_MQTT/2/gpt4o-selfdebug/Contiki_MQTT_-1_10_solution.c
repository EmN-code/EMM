#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "contiki.h"
#include "mqtt.h"
#include <stdio.h>

#define BROKER_IP     "47.102.103.1"
#define PORT          1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)

autostart_processes(&mqtt_publisher_process);

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
static struct etimer publish_timer;

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    static struct mqtt_connection conn;
    mqtt_register(&conn, &mqtt_publisher_process, "nrf52840", NULL, 64);
    mqtt_set_username_password(&conn, "username", "password"); // Set if authentication is needed

    printf("Connecting to MQTT broker...\n");
    while(!mqtt_connected(&conn)) {
        mqtt_connect(&conn, BROKER_IP, PORT, 1000);  // Timeout set to 1000 clock ticks
        PROCESS_PAUSE();  // Avoid busy-waiting
    }

    printf("Connected to MQTT broker\n");
    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        static char *publish_topic = "device/state";
        static char *message = "work";
        printf("Publishing message: %s to topic: %s\n", message, publish_topic);
        mqtt_publish(&conn, NULL, publish_topic, message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}