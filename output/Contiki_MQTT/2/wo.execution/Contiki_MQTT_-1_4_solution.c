#include "net/netstack.h"
#include "net/routing/routing.h"
#include "sys/etimer.h"
#include "contiki.h"
#include "mqtt.h"

#define SERVER_IP_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "nrf52840_contiki_mqtt"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)  // 5 seconds

static struct mqtt_connection conn;
static struct etimer publish_timer;
static char pub_topic[] = "nrf52840/status";
static char pub_message[20];

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    PROCESS_BEGIN();

    static mqtt_status_t status;
    static char broker_address[64];
    
    snprintf(broker_address, sizeof(broker_address), "tcp://%s:%d", SERVER_IP_ADDR, SERVER_PORT);
    mqtt_register(&conn, &mqtt_client_process, CLIENT_ID, NULL, MQTT_MAX_PACKET_SIZE);
    
    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {

        PROCESS_YIELD();

        if(ev == PROCESS_EVENT_TIMER && data == &publish_timer) {
            snprintf(pub_message, sizeof(pub_message), "work");  // Set your message
            status = mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)pub_message, strlen(pub_message), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);

            if(status != MQTT_STATUS_OK) {
                printf("Failed to publish message: %d\n", status);
            }

            etimer_reset(&publish_timer);
        }
    }

    PROCESS_END();
}

void mqtt_event(struct mqtt_connection *m, mqtt_event_t event, void *data)
{
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connection established\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT connection lost\n");
            break;
        default:
            printf("MQTT event received: %d\n", event);
            break;
    }
}
