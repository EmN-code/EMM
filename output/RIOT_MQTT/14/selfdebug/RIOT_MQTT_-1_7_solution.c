#include <ztimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define SEND_INTERVAL 5

static char msg_buf[128];
static char topic[] = "esp32/status";

static void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    MQTTPublish(client, topic, &message);
}

int main(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the network
    NetworkConnect(&network, SERVER_ADDR, SERVER_PORT);

    // Connect to the MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if(MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every SEND_INTERVAL seconds
    while (1) {
        publish_state(&client, "work");
        ztimer_sleep(ZTIMER_SEC, SEND_INTERVAL);
    }

    // Disconnect from MQTT broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}