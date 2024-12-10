
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/mqtt.h"
#include "net/sock/tcp.h"

#define BROKER_HOST "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static struct mqtt_connection conn;
static uint8_t payload[10];

void publish_state(void)
{
    static int state = 0;
    if (state) {
        strcpy((char *)payload, PAYLOAD_WORK);
    } else {
        strcpy((char *)payload, PAYLOAD_NOT_WORK);
    }
    state = !state;

    mqtt_publish(&conn, NULL, TOPIC, payload, strlen((char *)payload), MQTT_QOS_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_EN_OFF, NULL);
}

int main(void)
{
    sock_tcp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_HOST, &broker.addr.ipv4);

    mqtt_connect(&conn, (char *)BROKER_HOST, BROKER_PORT, 60, MQTT_CLEAN_SESSION_ON, NULL);

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
