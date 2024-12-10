#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <stddef.h>
#include <unistd.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <event/thread.h>
#include <mutex.h>
#include <mbox.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <net/protnum.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <thread.h>
#include <net/gcoap.h>
#include <architecture.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "esp32/status"
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not_work"
#define MQTT_PAYLOAD_LENGTH 9
#define MQTT_QOS 1
#define MQTT_TIMEOUT 10000L

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    int rc;

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = 0;
    message.payload = (void*)MQTT_PAYLOAD_WORK;
    message.payloadlen = MQTT_PAYLOAD_LENGTH;

    NetworkInit(&network);

    // Reconnect logic for network connection
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, MQTT_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Reconnect logic for MQTT connection
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connect failed. Retrying...\n");
            NetworkDisconnect(&network);
            xtimer_sleep(1);
            continue;
        } else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }

    while (1) {
        // Toggle work status
        message.payload = ((char*)message.payload == MQTT_PAYLOAD_WORK) ? (void*)MQTT_PAYLOAD_NOT_WORK : (void*)MQTT_PAYLOAD_WORK;

        // Publish message
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Error publishing message.\n");
        } else {
            printf("Message published: %s\n", (char*)message.payload);
        }

        xtimer_sleep(5);  // Wait for 5 seconds
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}