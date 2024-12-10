#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <net/gcoap.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 256
#define KEEP_ALIVE_INTERVAL 60

int main(void)
{
    struct sockaddr_in brokerAddr;
    int mqtt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mqtt_sock < 0) {
        puts("Failed to create socket.");
        return -1;
    }

    brokerAddr.sin_family = AF_INET;
    brokerAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &brokerAddr.sin_addr) <= 0) {
        puts("Invalid address or Address not supported.");
        return -1;
    }

    if (connect(mqtt_sock, (struct sockaddr *)&brokerAddr, sizeof(brokerAddr)) < 0) {
        puts("Connection Failed.");
        return -1;
    }

    // Prepare a fake MQTT CONNECT message (simplified)
    char connectMsg[] = "10 13 00 06 MQTT 04 02 00 3C 00 00"; // Example message, in reality you need to construct a proper MQTT message
    send(mqtt_sock, connectMsg, strlen(connectMsg), 0);

    while (1) {
        // Prepare a simple MQTT PUBLISH message
        char publishMsg[BUFFER_SIZE];
        snprintf(publishMsg, BUFFER_SIZE, "MQTT message: Device is working.");
        send(mqtt_sock, publishMsg, strlen(publishMsg), 0);
        sleep(5);
    }

    close(mqtt_sock);
    return 0;
}