#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <net/emcute.h>
#include <freertos/task.h>
#include <event/thread.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5
#define CLIENT_ID "esp32_client"

void mqtt_task(void *pvParameters) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;
    
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    while (1) {
        rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
        if (rc != 0) {
            printf("[ERROR] Network connection failed, retrying...\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        } else {
            printf("[INFO] Network connected successfully.\n");
            break;
        }
    }
    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    while ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("[ERROR] MQTT connect failed, retrying...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    printf("[INFO] Connected to MQTT Broker.\n");
    
    while (1) {
        MQTTMessage message;

        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)"work or not work";
        message.payloadlen = strlen("work or not work");

        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("[ERROR] Failed to publish message, retrying...\n");
            while ((rc = MQTTPublish(&client, "state", &message)) != 0) {
                printf("[ERROR] Retrying to publish message...\n");
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
        else {
            printf("[INFO] Message published successfully.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(PUBLISH_INTERVAL * 1000));
    }
}

int main(void) {
    xTaskCreate(mqtt_task, "mqtt_task", 2048, NULL, 1, NULL);
    vTaskStartScheduler();
    return 0;
}