#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uip.h"
#include "net/netstack.h"
#include "mqtt.h"
#include "random.h"
#include "sys/etimer.h"
#include "contiki.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5 * CLOCK_SECOND)
#define MQTT_BUFFER_SIZE 64

static struct etimer publish_timer;
static struct mqtt_connection conn;
static char mqtt_client_id[23];
static char pub_topic[] = "device/status";
static char pub_message[20];

static void mqtt_event_cb(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

void init_mqtt(void) {
    mqtt_register(&conn, NULL, mqtt_client_id, mqtt_event_cb, MQTT_BUFFER_SIZE);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, PUB_INTERVAL / CLOCK_SECOND, MQTT_CLEAN_SESSION_ON);
}

PROCESS(mqtt_publish_process, "MQTT Publish process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

PROCESS_THREAD(mqtt_publish_process, ev, data) {
    PROCESS_BEGIN();

    snprintf(mqtt_client_id, sizeof(mqtt_client_id), "mqtt_client_%02x", (unsigned int)(random_rand() % 256));

    init_mqtt();

    etimer_set(&publish_timer, PUB_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        snprintf(pub_message, sizeof(pub_message), "Device is %s", random_rand() % 2 == 0 ? "working" : "not working");

        mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

        printf("Published: %s\n", pub_message);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}