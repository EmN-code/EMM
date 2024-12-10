#include "ztimer.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "net/sock/udp.h"
#include "periph/gpio.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * US_PER_SEC)  // 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];
static const char* topic = "device/state";

void publish_state(void) {
    MQTTMessage message;
    char payload[20];

    // Alternate between "work" and "not work"
    static bool working = true;
    snprintf(payload, sizeof(payload), "status: %s", working ? "work" : "not work");
    working = !working;

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(&client, topic, &message);
    if (rc != SUCCESS) {
        printf("Publish failed with return code %d\n", rc);
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 5000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Unable to connect to broker at %s:%d\n", BROKER_IP, BROKER_PORT);
        return rc;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot-esp32";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("MQTT connect failed with return code %d\n", rc);
        return rc;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL / 1000000U);
    }

    return 0;
}