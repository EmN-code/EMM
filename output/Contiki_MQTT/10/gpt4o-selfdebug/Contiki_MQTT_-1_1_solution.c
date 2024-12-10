#include "sys/etimer.h"
#include "sys/clock.h"
#include "mqtt.h"
#include "netstack.h"
#include "contiki.h"
#include <stdio.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_CLIENT_ID "nrf52840_client"
#define MQTT_TOPIC "device/state"

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct etimer publish_timer;
static mqtt_client_t client;
static mqtt_connect_info_t connect_info = {
    .client_id = MQTT_CLIENT_ID,
    .user = "",              // Use if authentication is needed
    .password = ""          // Use if authentication is needed
};

PROCESS_THREAD(mqtt_client_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&client, &mqtt_client_process, MQTT_CLIENT_ID, NULL);
    connect_info.keep_alive_timer_s = 60;

    printf("Connecting to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    mqtt_connect(&client, BROKER_IP, BROKER_PORT, 1000, &connect_info);

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));
        const char* message = "work";
        printf("Publishing message: %s to topic %s\n", message, MQTT_TOPIC);
        mqtt_publish(&client, NULL, MQTT_TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}
