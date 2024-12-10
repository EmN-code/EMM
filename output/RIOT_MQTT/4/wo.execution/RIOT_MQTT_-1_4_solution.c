#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "dht_params.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];
static const char *status = "work";
static mutex_t mqtt_mutex = MUTEX_INIT;

int main(void) {
    printf("Starting MQTT example on ESP32\n");

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Error: Connection to network failed\n");
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Error: Connection to MQTT broker failed, return code %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mutex_lock(&mqtt_mutex);

        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)status;
        message.payloadlen = strlen(status);

        rc = MQTTPublish(&client, "status/topic", &message);

        mutex_unlock(&mqtt_mutex);

        if (rc != 0) {
            printf("Error: Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published: %s\n", status);
        }

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return 0;
}