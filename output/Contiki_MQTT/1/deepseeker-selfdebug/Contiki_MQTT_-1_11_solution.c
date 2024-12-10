
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "mqtt.h"
#include "net/netdev.h"
#include "net/netdev/eth.h"
#include "net/netif.h"
#include "net/ipv6/addr.h"

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS MQTT_QOS_0

static struct mqtt_connection conn;
static char client_id[] = "nrf52840_client";

void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
    (void)conn;
    (void)data;

    switch (event) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT connected\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT disconnected\n");
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT message published\n");
            break;
        default:
            break;
    }
}

int main(void) {
    netdev_t *netdev = netdev_get(0);
    if (netdev == NULL) {
        printf("No network device found\n");
        return 1;
    }

    netdev_register(netdev, NETDEV_ETH_TYPE_DEFAULT, 0);
    netif_set_default(netif_iter(NULL));

    struct mqtt_prop_list prop_list;
    mqtt_prop_list_init(&prop_list);

    mqtt_status_t status = mqtt_register(&conn, NULL, client_id, mqtt_event_callback, 128);
    if (status != MQTT_STATUS_OK) {
        printf("MQTT registration failed\n");
        return 1;
    }

    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, 1, &prop_list);
    if (status != MQTT_STATUS_OK) {
        printf("MQTT connection failed\n");
        return 1;
    }

    uint16_t mid;
    while (1) {
        status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_PAYLOAD, strlen(MQTT_PAYLOAD), MQTT_QOS, 0, 0, MQTT_TOPIC_ALIAS_EN_NO, &prop_list);
        if (status != MQTT_STATUS_OK) {
            printf("MQTT publish failed\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}
