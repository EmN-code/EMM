#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "architecture.h"
#include "event/thread.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "stdio.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "mutex.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_CLIENT_ID "esp32-client"
#define COMMAND_TIMEOUT_MS 4000

MQTTClient client;
Network network;
unsigned char sendbuf[80], readbuf[80];

void* message_queue[8];

void publisher(void)
{
    NetworkInit(&network);
    int rc = NetworkConnect(&network, SERVER_ADDR, SERVER_PORT);
    if (rc != 0) {
        printf("Error: Could not connect to the server.\n");
        return;
    }

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Error: Could not connect to MQTT broker. Return code: %d\n", rc);
        return;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)MQTT_PAYLOAD;
        message.payloadlen = strlen(MQTT_PAYLOAD);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Error: Could not publish message. Return code: %d\n", rc);
            return;
        }

        xtimer_sleep(5);
    }
}

int main(void)
{
    puts("ESP32 MQTT Publisher\n");

    msg_init_queue(message_queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publisher, NULL, "publisher");

    return 0;
}