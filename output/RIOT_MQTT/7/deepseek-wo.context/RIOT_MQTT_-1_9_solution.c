
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define INTERVAL_MS 5000000

static MQTTClient client;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = MQTT_PAYLOAD_LEN;

    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc;
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_mqtt_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
