
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
#define MQTT_MESSAGE "work"
#define MQTT_QOS MQTT_QOS_0
#define MQTT_RETAIN MQTT_RETAIN_FALSE
#define MQTT_TOPIC_ALIAS_EN MQTT_TOPIC_ALIAS_EN_FALSE
#define MQTT_KEEP_ALIVE 60

static struct mqtt_connection conn;
static struct netdev *netdev;
static struct netif *netif;

static void mqtt_event_callback(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
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

static void publish_state(void) {
    uint16_t mid;
    mqtt_status_t status = mqtt_publish(&conn, &mid, MQTT_TOPIC, (uint8_t *)MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN, 0, MQTT_TOPIC_ALIAS_EN, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT publish failed with status %d\n", status);
    } else {
        printf("MQTT publish successful\n");
    }
}

int main(void) {
    netdev = netdev_get(0);
    if (!netdev) {
        printf("Failed to get netdev\n");
        return 1;
    }

    netif = netif_iter(NULL);
    if (!netif) {
        printf("Failed to get netif\n");
        return 1;
    }

    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, "fe80::1");
    netif_set_opt(netif, NETOPT_IPV6_ADDR, 0, &addr, sizeof(addr));

    mqtt_status_t status = mqtt_register(&conn, NULL, "nrf52840", mqtt_event_callback, 0);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT register failed with status %d\n", status);
        return 1;
    }

    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_KEEP_ALIVE, 1, NULL);
    if (status != MQTT_STATUS_SUCCESS) {
        printf("MQTT connect failed with status %d\n", status);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
