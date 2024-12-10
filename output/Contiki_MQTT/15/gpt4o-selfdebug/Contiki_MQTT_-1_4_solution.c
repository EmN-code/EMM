#include <stdio.h>
#include <stdint.h>
#include "mqtt.h"
#include "net/ipv4/uc.h"
#include "dev/leds.h"
#include "process.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    static char payload[20];
    static uint16_t mid;
    
    PROCESS_BEGIN();
    
    mqtt_register(&conn, &mqtt_publisher_process, "contiki-nrf52840-client",
                  NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 1000, 1);
    
    etimer_set(&publish_timer, PUBLISH_INTERVAL);
    while(1) {
        PROCESS_YIELD();

        if (ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            snprintf(payload, sizeof(payload), "work or not work");
            mqtt_publish(&conn, &mid, "nrf52840/status", 
                         (uint8_t *)payload, strlen(payload), 
                         MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            
            etimer_reset(&publish_timer);
        }
    }
    
    PROCESS_END();
}