#include "socket.h"
#include "mqtt.h"
#include "contiki.h"

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

static struct etimer publish_timer;
static struct mqtt_connection conn;

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)
#define CLIENT_ID "nrf52840_client"
#define TOPIC "nrf52840/state"

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_client_process, CLIENT_ID, NULL, MAX_TCP_SEGMENT_SIZE);

    static char client_id[] = CLIENT_ID;
    static char* broker_ip = MQTT_BROKER_IP;
    static uint16_t broker_port = MQTT_BROKER_PORT;

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    conn.auto_reconnect = 1;

    mqtt_connect(&conn, broker_ip, broker_port, 100, MQTT_CLEAN_SESSION_ON);

    while(1) {
        PROCESS_YIELD();
        if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            static char message[20];
            snprintf(message, sizeof(message), "work");  // or "not work", depending on logic

            mqtt_publish(&conn, NULL, TOPIC, (uint8_t *)message, strlen(message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}