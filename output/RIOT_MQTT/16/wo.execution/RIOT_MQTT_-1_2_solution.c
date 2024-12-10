#include <xtimer.h>
#include "MQTTClient.h"
#include "paho_mqtt.h"

static char sendbuf[128];
static char readbuf[128];

int main(void)
{
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    int rc = 0;
    char * address = "47.102.103.1";
    int port = 1883;
    
    network.my_socket = 0;
    NetworkConnect(&network, address, port);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "esp32_client";

    if ((rc = MQTTConnect(&client, &connect_data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work";
    message.payloadlen = strlen("work");

    while (1) {
        if ((rc = MQTTPublish(&client, "esp32/status", &message)) != 0) {
            printf("Failed to publish, return code %d\n", rc);
        }
        xtimer_sleep(5);  // Delay for 5 seconds
    }

    return 0;
}