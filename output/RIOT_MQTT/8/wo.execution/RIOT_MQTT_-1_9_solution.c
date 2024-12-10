#include <timex.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <msg.h>
#include <paho_mqtt.h>

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");
    
    while (1) {
        if (MQTTPublish(client, "state", &message) != 0) {
            printf("Failed to publish message\n");
        }
        sleep(5);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    
    NetworkInit(&network);
    if (NetworkConnect(&network, "47.102.103.1", 1883) != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }
    
    publish_state(&client);

    return 0;
}