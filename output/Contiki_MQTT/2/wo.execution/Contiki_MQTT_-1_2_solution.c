#include "sys/etimer.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-default-conf.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include "udp-socket.h"
#include "tcp-socket.h"

#include <string.h>

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define CLIENT_ID "nrf52840_client"
#define TOPIC "device/state"

static struct mqtt_connection conn;
static struct etimer publish_timer;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    printf("MQTT Example started\n");

    mqtt_register(&conn, &mqtt_publish_process, CLIENT_ID, NULL, MAX_TCP_SEGMENT_SIZE);
    mqtt_set_username_password(&conn, "username", "password");

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while (1) {

        mqtt_connect(&conn, BROKER_IP_ADDR, BROKER_PORT, PUBLISH_INTERVAL / CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        char *work_status = "work";

        mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)work_status, strlen(work_status), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}