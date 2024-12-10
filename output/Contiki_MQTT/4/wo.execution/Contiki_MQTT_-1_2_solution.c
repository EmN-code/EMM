#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/netstack.h"
#include "stdio.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/routing/routing.h"
#include "contiki.h"
#include "mqtt-prop.h"
#include "contiki-net.h"

#define MQTT_KEEP_ALIVE 60
#define MQTT_CONNECT_TRIES 3

static struct mqtt_connection conn;
static char client_id[32];

PROCESS(nrf_mqtt_process, "NRF MQTT Process");
AUTOSTART_PROCESSES(&nrf_mqtt_process);

static mqtt_event_callback_t mqtt_event_callback;

static void mqtt_event_callback(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    if(event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT Broker\n");
    } else if(event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT Broker\n");
    }
}

PROCESS_THREAD(nrf_mqtt_process, ev, data) {
    PROCESS_BEGIN();
    static struct etimer timer;
    mqtt_status_t status;
    unsigned connection_tries = 0;
    snprintf(client_id, sizeof(client_id), "nrf-MQTT-client");
    mqtt_register(&conn, &nrf_mqtt_process, client_id, mqtt_event_callback, MQTT_KEEP_ALIVE);
    mqtt_set_username_password(&conn, "", "");

    while(connection_tries < MQTT_CONNECT_TRIES) {
        status = mqtt_connect(&conn, "47.102.103.1", 1883, MQTT_KEEP_ALIVE, 1, NULL);
        if(status == MQTT_STATUS_OK) {
            break;
        }
        printf("MQTT connection failed, retrying...\n");
        connection_tries++;
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_YIELD_UNTIL(etimer_expired(&timer));
    }

    if(status == MQTT_STATUS_OK) {
        while(1) {
            etimer_set(&timer, CLOCK_SECOND * 5);
            PROCESS_YIELD_UNTIL(etimer_expired(&timer));
            char payload[20];
            snprintf(payload, sizeof(payload), "work");
            mqtt_publish(&conn, NULL, "device/state", (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, 0, 0, 0, NULL);
            printf("Published state: %s\n", payload);
        }
    } else {
        printf("Failed to connect to MQTT broker after %d attempts\n", MQTT_CONNECT_TRIES);
    }

    PROCESS_END();
}
