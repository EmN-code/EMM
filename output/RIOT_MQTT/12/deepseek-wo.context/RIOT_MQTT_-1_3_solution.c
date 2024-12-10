
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static MQTTClient client;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    /* Initialize the network structure */
    NetworkInit(&network);

    /* Connect to the MQTT broker */
    int result = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    /* Initialize the MQTT client */
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Connect to the MQTT broker */
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    result = MQTTConnect(&client, &options);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
