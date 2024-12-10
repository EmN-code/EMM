#include "uip-udp-packet.h"
#include "udp-socket.h"
#include "tcp-socket.h"
#include "mqtt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct mqtt_connection conn;
char client_id[] = "nrf52840_client";

PROCESS(mqtt_example, "MQTT Example");
AUTOSTART_PROCESSES(&mqtt_example);

static void 
init_mqtt_connection(void) {
    mqtt_register(&conn, &mqtt_example, client_id, mqtt_event_handler, 64);
    mqtt_connect(&conn, "47.102.103.1", 1883, 60, 1, NULL); // Using '1' for clean_session
}

static void 
publish_state(void) {
    uint16_t mid;
    char topic[] = "device/state";
    char payload[] = "work";
    mqtt_publish(&conn, &mid, topic, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0);
}

PROCESS_THREAD(mqtt_example, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();
    init_mqtt_connection();

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        publish_state();
    }

    PROCESS_END();
}

static void 
mqtt_event_handler(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    // Placeholder for event handling logic, if needed
    printf("MQTT event: %d\n", event);
}