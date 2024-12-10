#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcp-socket.h"
#include "contiki.h"
#include "mqtt.h"

#define MAX_TCP_SEGMENT_SIZE 256
#define MQTT_QOS_LEVEL_0 0

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_test_process, "MQTT Test Process");
AUTOSTART_PROCESSES(&mqtt_test_process);

PROCESS_THREAD(mqtt_test_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_status_t status;
    uint16_t mid;

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    // Initialize the MQTT client
    mqtt_register(&conn, &mqtt_test_process, "client_id", NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, "47.102.103.1", 1883, 60);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&periodic_timer)) {
            const char *topic = "device/state";
            const char *msg = "work or not work";
            status = mqtt_publish(&conn, &mid, topic, (uint8_t *)msg, strlen(msg), MQTT_QOS_LEVEL_0);
            if(status == MQTT_STATUS_OK) {
                printf("Publish ok\n");
            }
            else {
                printf("Publish failed with status: %d\n", status);
            }

            etimer_reset(&periodic_timer);
        }
    }

    PROCESS_END();
}
