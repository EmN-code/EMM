#include "net/routing/routing.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include "sys/etimer.h"
#include "random.h"
#include "contiki.h"

#define LOG_MODULE "MQTT"
#define LOG_LEVEL LOG_LEVEL_INFO

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (CLOCK_SECOND * 5)
#define MQTT_CLIENT_ID "nrf52840-mqtt-client"
#define MQTT_PUB_TOPIC "device/state"

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char state_message[16];

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data) {
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            LOG_INFO("Connected to broker\n");
            mqtt_status_t status = mqtt_subscribe(&conn, NULL, MQTT_PUB_TOPIC, MQTT_QOS_LEVEL_0);
            if(status != MQTT_STATUS_OK) {
                LOG_ERR("Failed to subscribe\n");
            }
            break;
        case MQTT_EVENT_DISCONNECTED:
            LOG_INFO("Disconnected from broker\n");
            break;
        default:
            LOG_ERR("MQTT event received: %d\n", event);
            break;
    }
}

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    mqtt_register(&conn, &mqtt_example_process, MQTT_CLIENT_ID, mqtt_event, MQTT_MAX_SEGMENT_SIZE);
    mqtt_set_username_password(&conn, "user", "pass");
    mqtt_connect(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_PUBLISH_INTERVAL / CLOCK_SECOND);

    etimer_set(&publish_timer, MQTT_PUBLISH_INTERVAL);

    while(1) {
        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            snprintf(state_message, sizeof(state_message), "work");
            mqtt_publish(&conn, NULL, MQTT_PUB_TOPIC, (uint8_t *)state_message, strlen(state_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}