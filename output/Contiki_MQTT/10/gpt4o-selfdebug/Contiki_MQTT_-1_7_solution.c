#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "sys/etimer.h"
#include "contiki-net.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUB_TOPIC "nrf52840/state"
#define MQTT_CLIENT_ID "nrf52840_client"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(nrf52840_mqtt_process, "NRF52840 MQTT Publisher");
AUTOSTART_PROCESSES(&nrf52840_mqtt_process);

static struct mqtt_message pub_msg;
static struct mqtt_connection conn;
static struct etimer et;
static char pub_payload[20];

void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Publish complete\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

PROCESS_THREAD(nrf52840_mqtt_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &nrf52840_mqtt_process, MQTT_CLIENT_ID, mqtt_event_callback, 256);
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, PUBLISH_INTERVAL, MQTT_CLEAN_SESSION_ON);

    etimer_set(&et, PUBLISH_INTERVAL);
    while(1) {
        PROCESS_YIELD();
        if (ev == PROCESS_EVENT_TIMER) {
            snprintf(pub_payload, sizeof(pub_payload), "work");
            pub_msg.payload = (uint8_t *)pub_payload;
            pub_msg.payload_length = strlen(pub_payload);
            mqtt_publish(&conn, NULL, MQTT_PUB_TOPIC, pub_msg.payload, pub_msg.payload_length,
                         MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&et);
        }
    }

    PROCESS_END();
}