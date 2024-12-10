#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include <sys/socket.h>  // To enable socket communication
#include <netinet/in.h>
#include <unistd.h>      // Provides access to the POSIX operating system API
#include "mqtt_paho.h"  // Using the Paho MQTT library
#include "net/sock/udp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "time_units.h"
#include "timex.h"
#include "xtimer.h"
#include "thread.h"     // To utilize threading operations
#include "ztimer.h"
#include "net/gcoap.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "net/sock/util.h"
#include "architecture.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "mutex.h"
#include "dht_params.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC   "state"
#define PUB_MSG     "work"
#define PUB_INTERVAL (5 * 1000000U)  // Publish every 5 seconds

static void mqtt_task(void *pvParameters) {
    (void)pvParameters;

    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("MQTT connect failed with rc: %d\n", rc);
        return;
    }

    printf("MQTT connected\n");

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)PUB_MSG;
        message.payloadlen = strlen(PUB_MSG);

        if ((rc = MQTTPublish(&client, PUB_TOPIC, &message)) != 0) {
            printf("MQTT publish failed with rc: %d\n", rc);
        } else {
            printf("Message published: %s\n", PUB_MSG);
        }

        vTaskDelay(pdMS_TO_TICKS(PUB_INTERVAL));
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
}

int main(void) {
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;
    const portSHORT stackSize = 1024;

    xReturned = xTaskCreate(
                    mqtt_task,
                    "MQTT Task",
                    stackSize,
                    NULL,
                    tskIDLE_PRIORITY + 1,
                    &xHandle);

    if (xReturned == pdPASS) {
        vTaskStartScheduler();
    } else {
        printf("Task creation failed\n");
    }

    return 0;
}