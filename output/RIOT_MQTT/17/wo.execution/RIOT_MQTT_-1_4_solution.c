#include "netinet/in.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "esp_system.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "msg.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U)

static unsigned char sendbuf[128];
static unsigned char readbuf[128];

int main(void) {
    printf("Starting MQTT client example\n");

    Network network;
    MQTTClient client;
    int rc;

    // Initialize network structure
    rc = NetworkConnect(&network, MQTT_BROKER, MQTT_PORT);
    if (rc != 0) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Configure MQTT message
    MQTTMessage message;
    char payload[] = "work";
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    message.qos = 0;
    message.retained = 0;

    while (1) {
        rc = MQTTPublish(&client, "test/topic", &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        }
        sleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}
