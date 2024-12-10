#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread.h>
#include <mutex.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "state"
#define QOS         1
#define TIMEOUT     10000L

MQTTClient client;
Network network;

void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *) state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, TOPIC, &message);
}

int main() {
    char *states[2] = {"work", "not work"};
    int state_index = 0;
    int rc;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    NetworkConnect(&network, "47.102.103.1", 1883);

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData conn_data = MQTTPacket_connectData_initializer;
    conn_data.clientID.cstring = CLIENTID;
    conn_data.keepAliveInterval = 20;
    conn_data.cleansession = 1;
    rc = MQTTConnect(&client, &conn_data);

    if (rc != 0) {
        printf("Unable to connect, return code %d\n", rc);
        return -1;
    }

    while (1) {
        publish_state(states[state_index]);
        state_index = (state_index + 1) % 2;
        sleep(5);
    }

    return 0;
}