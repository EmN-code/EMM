#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <mutex.h>  // For managing mutex with MQTT network and client
#include "freertos/task.h"  // Required for task management in FreeRTOS
#include <esp_system.h> // For ESP32-specific system functions
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <evtimer_mbox.h>
#include <net/sock/util.h>
#include <stdio.h>
#include <timex.h>
#include <net/gcoap.h>
#include "mbox.h"
#include <net/emcute.h>
#include "netinet/in.h"
#include "mutex.h"
#include "esp_system.h"
#include "free_rtos.h"
#include "event/thread.h"
#include "MQTTClient.h"  // Include last as required

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/status"
#define MESSAGE_INTERVAL 5000

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

static const char *work_status[] = {"work", "not work"};

void publish_task(void *pvParameters) {
    int rc;
    while (1) {
        static int status_index = 0;
        MQTTMessage message;
        char payload[20];

        snprintf(payload, sizeof(payload), "%s", work_status[status_index]);
        message.qos = QOS0;
        message.retained = 0;
        message.payload = payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if(rc != 0) {
            printf("Publish failed with return code %d\n", rc);
        } else {
            printf("Published: %s\n", payload);
        }

        status_index = (status_index + 1) % 2;
        vTaskDelay(MESSAGE_INTERVAL / portTICK_PERIOD_MS);
    }
}

int main(void) {
    int rc;
    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connection failed with return code %d\n", rc);
        return -1;
    }

    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connection failed with return code %d\n", rc);
        return -1;
    }

    printf("Starting publish task\n");
    xTaskCreate(publish_task, "pubTask", 2048, NULL, 1, NULL);

    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    return 0;
}