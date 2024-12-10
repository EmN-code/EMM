#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "esp_event.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];

typedef struct {
    sock_udp_ep_t gw;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[64], readbuf[64];
} mqtt_params_t;

static void *mqtt_thread(void *arg) {
    mqtt_params_t *params = (mqtt_params_t *)arg;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32_client";

    int rc = MQTTConnect(&params->client, &data);
    if (rc != 0) {
        printf("Failed to connect to the broker\n");
        return NULL;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "not work";
    message.payloadlen = strlen(message.payload);

    while (1) {
        rc = MQTTPublish(&params->client, "device/state", &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message\n");
        }

        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);

        // Toggle state
        if (strcmp(message.payload, "not work") == 0) {
            message.payload = "work";
        } else {
            message.payload = "not work";
        }
    }
    return NULL;
}

int main(void) {
    mqtt_params_t mqtt_params;

    NetworkInit(&mqtt_params.network);
    MQTTClientInit(&mqtt_params.client, &mqtt_params.network, 1000, 
                   mqtt_params.sendbuf, sizeof(mqtt_params.sendbuf), 
                   mqtt_params.readbuf, sizeof(mqtt_params.readbuf));

    mqtt_params.gw.family = AF_INET;
    mqtt_params.gw.port = BROKER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&mqtt_params.gw.addr, BROKER_IP);

    if (NetworkConnect(&mqtt_params.network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Failed to connect network\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, 
                  mqtt_thread, &mqtt_params, "mqtt_thread");

    return 0;
}