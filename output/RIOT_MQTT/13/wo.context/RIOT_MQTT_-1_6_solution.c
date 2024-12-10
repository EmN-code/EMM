#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/sock.h>
#include <stdio.h>
#include "MQTTClient.h"

#define ADDRESS     "47.102.103.1:1883"
#define CLIENTID    "ExampleClientID"
#define TOPIC       "state"
#define QOS         1
#define TIMEOUT     10000L

void setup_mqtt(MQTTClient *client, Network *network, char *addr_ip, int port) {
    // Network initialization
    NetworkInit(network);
    NetworkConnect(network, addr_ip, port);
    
    // MQTT Client initialization
    unsigned char sendbuf[80], readbuf[80];
    MQTTClientInit(client, network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;
    
    int rc = MQTTConnect(client, &connectData);
    if (rc != SUCCESS)
        printf("Failed to connect, return code %d\n", rc);

}

void publish_state(MQTTClient *client, const char *topic, const char *message) {
    MQTTMessage pubMessage;
    pubMessage.qos = QOS;
    pubMessage.retained = 0;
    pubMessage.payload = (void *)message;
    pubMessage.payloadlen = strlen(message);
    
    int rc = MQTTPublish(client, topic, &pubMessage);
    if (rc != SUCCESS)
        printf("Message publication failed, return code %d\n", rc);
}

int main(void) {
    Network network;
    MQTTClient client;
    const char *addr_ip = "47.102.103.1";
    int port = 1883;
    
    setup_mqtt(&client, &network, addr_ip, port);
    while (1) {
        publish_state(&client, TOPIC, "work");
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    return 0;
}