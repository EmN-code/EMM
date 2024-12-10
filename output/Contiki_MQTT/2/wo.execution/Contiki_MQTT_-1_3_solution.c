#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "mqtt.h"
#include "dht11-sensor.h"
#include "sys/etimer.h"
#include "dev/button-hal.h"
#include "dev/etc/rgb-led/rgb-led.h"

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define PUBLISH_INTERVAL  (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_publisher_process, "MQTT Publisher Process");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

PROCESS_THREAD(mqtt_publisher_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_publisher_process, "nrf52840_client",
                  NULL, MAX_TCP_SEGMENT_SIZE);

    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        if (mqtt_connected(&conn)) {
            char *topic = "nrf52840/status";
            char *message = "work";  // Example message to publish

            mqtt_publish(&conn, NULL, topic, (uint8_t *)message,
                         strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, NULL);
        }

        etimer_reset(&periodic_timer);
    }

    mqtt_disconnect(&conn);

    PROCESS_END();
}
