#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "mqtt.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "sys/etimer.h"
#include "stdio.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "nrf52840_client"
#define PUB_TOPIC "nrf52840/status"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer periodic_timer;

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

PROCESS_THREAD(mqtt_example_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, CLIENT_ID, NULL, 512);

    static mqtt_status_t status;
    status = mqtt_connect(&conn, BROKER_IP, BROKER_PORT, 60 * CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);
    if (status != MQTT_STATUS_OK) {
        printf("Failed to connect to MQTT broker.\n");
        PROCESS_EXIT();
    }

    etimer_set(&periodic_timer, PUBLISH_INTERVAL);
    static char sensor_status[20];

    while (1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

      if (dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER) > 0) {
        snprintf(sensor_status, sizeof(sensor_status), "work");
      } else {
        snprintf(sensor_status, sizeof(sensor_status), "not work");
      }

      status = mqtt_publish(&conn, NULL, PUB_TOPIC, (uint8_t *)sensor_status,
                            strlen(sensor_status), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

      if (status != MQTT_STATUS_OK) {
        printf("Failed to publish MQTT message.\n");
      }

      etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}
