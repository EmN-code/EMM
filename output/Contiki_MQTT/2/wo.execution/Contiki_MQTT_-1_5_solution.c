#include "sys/etimer.h"
#include "contiki.h"
#include "mqtt.h"
#include "net/netstack.h"
#include "random.h"
#include "string.h"

#define MQTT_BROKER_IP_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT       1883
#define MQTT_PUB_TOPIC         "nrf52840/state"
#define MQTT_CLIENT_ID         "nrf52840"
#define MQTT_MAX_PACKET_SIZE   128
#define MQTT_PUBLISH_INTERVAL  (5 * CLOCK_SECOND)

PROCESS(mqtt_state_publish_process, "MQTT State Publish Process");
AUTOSTART_PROCESSES(&mqtt_state_publish_process);

static struct mqtt_connection conn;
static struct etimer publish_timer;

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event,
                                void *data) {
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

PROCESS_THREAD(mqtt_state_publish_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_state_publish_process, MQTT_CLIENT_ID,
                  mqtt_event_callback, MQTT_MAX_PACKET_SIZE);

    mqtt_connect(&conn, MQTT_BROKER_IP_ADDR, MQTT_BROKER_PORT, 100);

    etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&publish_timer)) {
            static char pub_message[16];
            int state = random_rand() % 2;  // Simulating "work" or "not work"
            snprintf(pub_message, sizeof(pub_message), "%s",
                     state == 1 ? "work" : "not work");

            printf("Publishing MQTT message: %s\n", pub_message);

            mqtt_publish(&conn, NULL, MQTT_PUB_TOPIC, (uint8_t *)pub_message,
                         strlen(pub_message), 0, 0);

            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}