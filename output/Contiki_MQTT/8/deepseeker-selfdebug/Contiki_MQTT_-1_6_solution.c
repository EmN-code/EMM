
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <mqtt.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEP_ALIVE 60
#define MQTT_QOS MQTT_QOS_0

static struct mqtt_connection conn;
static uint16_t mid;

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;
    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("MQTT publish event\n");
            break;
        default:
            break;
    }
}

int main(void) {
    mqtt_status_t status;

    /* Initialize the MQTT connection */
    status = mqtt_register(&conn, NULL, "nrf52840_client", mqtt_event_callback, 0);
    if (status != MQTT_OK) {
        printf("MQTT register failed\n");
        return -1;
    }

    /* Connect to the MQTT broker */
    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);
    if (status != MQTT_OK) {
        printf("MQTT connect failed\n");
        return -1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, MQTT_PAYLOAD_SIZE, MQTT_QOS, 0, 0, 0, NULL);
        if (status != MQTT_OK) {
            printf("MQTT publish failed\n");
        } else {
            printf("MQTT publish success\n");
        }
        xtimer_sleep(5);
    }

    /* Disconnect from the MQTT broker */
    mqtt_disconnect(&conn, NULL);

    return 0;
}
