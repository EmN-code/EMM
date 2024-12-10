
#include <stdio.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000

static Network network;
static MQTTClient client;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_message_arrived(MessageData* data) {
    (void)data;
}

int main(void) {
    // Initialize the network
    NetworkInit(&network);

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Convert the IP address string to a byte-represented IP address
    struct sockaddr_in remote;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.sin_addr);
    remote.sin_family = AF_INET;
    remote.sin_port = htons(EMQX_SERVER_PORT);

    // Connect to the MQTT broker
    if (NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Set up MQTT connection options
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    // Connect to the MQTT broker
    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = MQTT_QOS;
        message.retained = MQTT_RETAIN;
        message.payload = (void*)MQTT_PAYLOAD;
        message.payloadlen = strlen(MQTT_PAYLOAD);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect from the MQTT broker
    NetworkDisconnect(&network);

    return 0;
}
