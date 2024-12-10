#include <sys/socket.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <kernel_defines.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STACK_SIZE 4096
#define PRIORITY 1
#define MESSAGE "work"

Network network;
MQTTClient client;
unsigned char sendbuf[100], readbuf[100];

void mqtt_pub_task(void *pvParameters) {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT-Client";

    while (true) {
        while (1) {
            rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
            if (rc != 0) {
                printf("Network connection failed\n");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            } else {
                printf("Network connected\n");
                break;
            }
        }

        while (1) {
            rc = MQTTConnect(&client, &connectData);
            if (rc != 0) {
                printf("MQTT connection failed\n");
                NetworkDisconnect(&network);
                vTaskDelay(pdMS_TO_TICKS(1000));
                break;
            } else {
                printf("MQTT connected\n");
                break;
            }
        }

        while (1) {
            MQTTMessage message;
            message.qos = 1;
            message.retained = 0;
            message.dup = 0;
            message.payload = (void *)MESSAGE;
            message.payloadlen = strlen(MESSAGE);

            while (1) {
                rc = MQTTPublish(&client, "state_topic", &message);
                if (rc != 0) {
                    printf("Publish failed\n");
                    MQTTDisconnect(&client);
                    NetworkDisconnect(&network);
                    vTaskDelay(pdMS_TO_TICKS(1000));
                    break;
                } else {
                    printf("Publish successful\n");
                    break;
                }
            }
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    xTaskCreate(mqtt_pub_task, "mqtt_pub_task", STACK_SIZE, NULL, PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}