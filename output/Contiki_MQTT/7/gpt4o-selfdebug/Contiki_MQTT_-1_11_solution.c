#include "net/ipv6/uip.h"
#include "mqtt_status_t mqtt_auth.h"
#include "mqtt_status_t mqtt_publish.h"
#include "mqtt_status_t mqtt_connect.h.h"
#include <stdio.h>

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Process");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

PROCESS_THREAD(nrf52840_mqtt_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_status_t status;
    char *broker_ip = "47.102.103.1";
    uint16_t port = 1883;
    char *topic = "state";
    char *payload_work = "work";
    char *payload_not_work = "not work";

    etimer_set(&et, CLOCK_SECOND * 5); // Set the timer to 5 seconds

    status = mqtt_connect(&conn, broker_ip, port, CLOCK_SECOND * 5, MQTT_CLEAN_SESSION);

    while(1) {
        PROCESS_WAIT_EVENT();

        if(etimer_expired(&et)) {
            mqtt_publish(&conn, NULL, topic, (uint8_t *)payload_work, strlen(payload_work), MQTT_QOS_LEVEL_0);
            etimer_reset(&et); // Reset the timer
        }
    }

    PROCESS_END();
}