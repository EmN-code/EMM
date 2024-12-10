#include <os/sys/etimer.h>
#include <os/sys/ctimer.h>
#include <os/net/ipv6/mqtt/client.h>
#include "contiki.h"

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct etimer timer;
static struct mqtt_client client;
static char *broker_ip = "47.102.103.1";
static char *topic = "device/state";

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&client, &mqtt_publisher_process, "nrf52840_client", NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_connect(&client, broker_ip, 1883, 100); // connect with host, port, keep-alive time

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        uint8_t state = 1; // Assume 1 is "work" and 0 is "not work"

        mqtt_publish(&client, NULL, topic, &state, sizeof(state), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&timer);
    }

    PROCESS_END();
}