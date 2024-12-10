
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "contiki-default-conf.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/routing/routing.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS MQTT_QOS_LEVEL_0

static struct mqtt_connection mqtt_conn;
static struct simple_udp_connection udp_conn;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("MQTT connected\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("MQTT disconnected\n");
    }
}

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    static struct etimer et;
    static uint16_t mid;

    PROCESS_BEGIN();

    mqtt_register(&mqtt_conn, PROCESS_CURRENT(), "nrf52840_client", mqtt_event_callback, 128);
    mqtt_connect(&mqtt_conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, 60, 1, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (mqtt_connected(&mqtt_conn)) {
            mqtt_publish(&mqtt_conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, sizeof(MQTT_PAYLOAD) - 1, MQTT_QOS, 0, 0, 0, NULL);
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&mqtt_publish_process, NULL);
    return 0;
}
