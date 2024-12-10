#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC)

static mqtt_client_t client;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };

int main(void) {
    printf("Starting MQTT example\n");

    mqtt_connect(&client, &gw, BROKER_IP, sizeof(BROKER_IP), true);
    
    mqtt_message_t message;
    char payload[20];

    while (1) {
        snprintf(payload, sizeof(payload), "%s", "work");
        message.payload = (uint8_t *)payload;
        message.payload_len = strlen(payload);
        message.topic = TOPIC;
        message.qos = 0;

        int res = mqtt_publish(&client, &message);
        if (res != 0) {
            printf("Error while publishing: %d\n", res);
        } else {
            printf("Message published: %s\n", payload);
        }

        xtimer_sleep(INTERVAL);  // Sleep for the specified interval
    }

    return 0;
}