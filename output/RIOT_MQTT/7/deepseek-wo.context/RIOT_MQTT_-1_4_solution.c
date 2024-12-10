
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[200];

    // Initialize the network
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    MQTTConnect(&client, &options);

    // Publish state every 5 seconds
    while (1) {
        publish_state(&client);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
