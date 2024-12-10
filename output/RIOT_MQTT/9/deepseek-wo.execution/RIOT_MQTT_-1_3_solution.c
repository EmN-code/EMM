
#include <stdio.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <MQTTClient.h>  // Ensure this is the last header file to be included

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 5000L

int main(void)
{
    // Initialize the MQTT client
    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[200];
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    IPStack ipstack;
    IPStackInit(&ipstack);
    IPStackConnect(&ipstack, SERVER_IP, SERVER_PORT);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Failed to publish, return code %d\n", rc);
        } else {
            printf("Published: %s\n", PAYLOAD);
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    // Disconnect from the MQTT broker
    if ((rc = MQTTDisconnect(&client)) != 0) {
        printf("Failed to disconnect, return code %d\n", rc);
    }

    IPStackDisconnect(&ipstack);

    return 0;
}
