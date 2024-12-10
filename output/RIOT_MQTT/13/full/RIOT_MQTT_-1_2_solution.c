#include "thread.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define BUFFER_SIZE 100
#define PUBLISH_INTERVAL (5U)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];

void *mqtt_publish_thread(void *arg) {
    (void)arg;
    char *topic = "device/state";
    MQTTMessage message;
    int rc;
    while (1) {
        char *payload = "work";  // Or "not work" depending on actual state
        message.qos = 1;
        message.retained = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, topic, &message);
        if (rc != 0) {
            printf("Failed to publish message, trying again in 1 second.\n");
        } else {
            printf("Message published to topic %s: %s\n", topic, payload);
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, mqtt_publish_thread, NULL, "mqtt_publisher");

    int rc;

    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying in 1 second... \n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to network successfully.\n");
            break;
        }
    }
    
    mqtt_publish_thread(NULL);

    return 0;
}