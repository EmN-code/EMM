#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ExampleClient"
#define TOPIC       "state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

int main(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    int rc = NetworkConnect(&network, "47.102.103.1", 1883);
    if (rc != SUCCESS) {
        printf("Failed to connect to network, return code %d\n", rc);
        return rc;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to broker, return code %d\n", rc);
        return rc;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void *)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
        }

        sleep(5);
    }

    return 0;
}