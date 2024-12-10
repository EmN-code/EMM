#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include "contiki.h"
#include <string.h>

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static char broker_ip[] = "47.102.103.1";
static uint16_t broker_port = 1883;
static char client_id[] = "nrf52840_client";
static char topic[] = "state";
static char message[] = "work or not work";

static void
timer_callback(void *ptr)
{
    mqtt_publish(&conn, NULL, topic, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_OFF, NULL);
}

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, client_id, NULL, MQTT_MAX_SEGMENT_SIZE);
    mqtt_connect(&conn, broker_ip, broker_port, 5 * CLOCK_SECOND, MQTT_CLEAN_SESSION_ON, NULL);

    etimer_set(&timer, 5 * CLOCK_SECOND);
    while(1) {
        PROCESS_YIELD();
        if(etimer_expired(&timer)) {
            timer_callback(NULL);
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}