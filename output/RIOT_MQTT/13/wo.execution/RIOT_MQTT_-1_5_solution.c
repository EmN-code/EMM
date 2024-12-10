#include <esp_system.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <architecture.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL 5
#define TOPIC "esp32/state"

static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_thread(void *arg) {
    (void)arg;

    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "esp32client";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to the MQTT broker.\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        memset(&message, 0, sizeof(message));
        message.qos = QOS0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Error while publishing message.\n");
        }

        sleep(PUBLISH_INTERVAL);
    }

    NetworkDisconnect(&network);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_thread, NULL, "mqtt_thread");
    return 0;
}
