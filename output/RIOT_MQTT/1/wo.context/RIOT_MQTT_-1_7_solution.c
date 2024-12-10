#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/protnum.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define MQTT_CLIENT_ID    "RIOT_ESP32_Client"
#define MQTT_TOPIC        "device/state"
#define MQTT_PAYLOAD      "work"
#define MQTT_PAYLOAD_IDLE "not work"
#define INTERVAL          (5U) // Publish interval in seconds

static MQTTClient client;
static Network network;

static void *publish_thread(void *arg) {
    (void) arg;
    int rc = 0;

    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("ERROR: Network connection failed\n");
        return NULL;
    }

    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("ERROR: MQTT connection failed with result code: %d\n", rc);
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*) MQTT_PAYLOAD;
        message.payloadlen = strlen(MQTT_PAYLOAD);

        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != SUCCESS) {
            printf("ERROR: MQTT publish failed with result code: %d\n", rc);
        } else {
            printf("MQTT published message: %s\n", MQTT_PAYLOAD);
        }

        sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), 0, 0, publish_thread, NULL, "pub_thread");
    return 0;
}