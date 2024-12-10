#include "net/ipv6/addr.h"
#include "mqtt.h"
#include "kernel.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * KERNEL_MSEC)

static char mqtt_client_id[64];
static struct mqtt_connection conn;

static void init_mqtt(void) {
    /* initialize mqtt client id with a unique identifier */
    snprintf(mqtt_client_id, sizeof(mqtt_client_id), "nrf52840_client_%02x", rand());
    
    /* initialize MQTT connection parameters */
    mqtt_status_t status = mqtt_register(&conn, &contiki_process,
                                         mqtt_client_id, mqtt_event_callback,
                                         100);
    
    if (status != MQTT_STATUS_OK) {
        printf("MQTT registration failed\n");
        return;
    }

    /* setup broker address */
    ipv6_addr_t broker_addr;
    ipv6_addr_from_str(&broker_addr, BROKER_IP);

    /* connect to MQTT broker */
    if (mqtt_connect(&conn, &broker_addr, BROKER_PORT, PUBLISH_INTERVAL) != MQTT_STATUS_OK) {
        printf("MQTT connection failed\n");
        return;
    }

    printf("MQTT connected to %s:%d\n", BROKER_IP, BROKER_PORT);
}

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    printf("MQTT event callback: %d\n", event);
}

static void publish_state(void) {
    char *state = "work"; // or "not work" depending on some condition
    uint16_t message_id = 0;
    mqtt_status_t status = mqtt_publish(&conn, &message_id, "state/topic", (uint8_t *)state,
                                        strlen(state), MQTT_QoS_0, MQTT_RETAIN_OFF);
    if (status != MQTT_STATUS_OK) {
        printf("MQTT publish failed\n");
    }
}

int main(void) {
    init_mqtt();

    while (1) {
        publish_state();
        kernel_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}
