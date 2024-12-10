
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize network
    NetworkInit(&network);
    NetworkConnect(&network, SERVER_IP, SERVER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_mqtt_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&client, &data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
