#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <MQTTPacket.h>
#include <lwip/sockets.h>

#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT 1883
#define PUBLISH_INTERVAL 5
#define TOPIC "device/state"
#define PAYLOAD "work"

int main(void) {
    int sock;
    struct sockaddr_in broker_addr;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTString topicString = MQTTString_initializer;
    unsigned char buf[200];
    int buf_len = sizeof(buf);
    int payload_len = strlen(PAYLOAD);

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_PORT);

    if (inet_pton(AF_INET, MQTT_BROKER, &broker_addr.sin_addr) <= 0) {
        printf("Error converting IP address\n");
        return -1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&broker_addr, sizeof(broker_addr)) < 0) {
        printf("Error connecting to broker\n");
        return -1;
    }

    int len = MQTTSerialize_connect(buf, buf_len, &connectData);
    send(sock, buf, len, 0);

    while (1) {
        topicString.cstring = TOPIC;
        len = MQTTSerialize_publish(buf, buf_len, 0, 0, 0, 0, topicString, (unsigned char*)PAYLOAD, payload_len);
        send(sock, buf, len, 0);

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * 1000);
    }

    return 0;
}