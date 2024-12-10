#include "esp_system.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "mutex.h"

#include "net/emcute.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "thread.h"

#include "MQTTClient.h"

#define MQTT_TOPIC "esp32/status"
#define MQTT_DATA "work"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

static kernel_pid_t main_thread_id;
static MQTTClient client;

void *mqtt_thread(void *arg) {
    (void)arg;

    while (1) {
        int rc;

        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        rc = MQTTConnect(&client, &network);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            NetworkDisconnect(&network);
            continue;
        }

        printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

        while (1) {
            rc = MQTTPublish(&client, MQTT_TOPIC, MQTT_DATA);
            if (rc != 0) {
                printf("MQTT publish failed. Retrying...\n");
                break;
            }
            printf("Published data: %s to topic: %s\n", MQTT_DATA, MQTT_TOPIC);

            vTaskDelay(pdMS_TO_TICKS(5000));
        }

        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    return NULL;
}

int main(void) {
    printf("ESP32 MQTT-SN example\n");

    main_thread_id = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    if (main_thread_id <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    /* Wait for messages to arrive on the message queue, this is a simple blocking
       wait approach */
    char buffer[64];
    while (1) {
        msg_t msg;
        if (msg_try_receive(&msg) == 1) {
            printf("Received message: %s\n", buffer);
        }
    }

    return 0;
}
