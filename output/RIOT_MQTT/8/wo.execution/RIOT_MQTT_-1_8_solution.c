#include <esp_wifi.h>
#include <esp_system.h>
#include <architecture.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <msg.h>
#include <mutex.h>
#include <thread.h>
#include <xtimer.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <timex.h>
#include <netinet/in.h>
#include <net/net.h>
#include <MQTTClient.h>

#define ADDRESS_IP "47.102.103.1"
#define PORT_NUMBER 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publisher_thread(void *arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    NetworkInit(&network);
    if (NetworkConnect(&network, ADDRESS_IP, PORT_NUMBER) != 0) {
        printf("Error connecting to the network\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "ESP32Client";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Error connecting to the MQTT broker\n");
        return NULL;
    }

    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)"work or not work";
    message.payloadlen = strlen((char *)message.payload);

    while (1) {
        int rc = MQTTPublish(&client, "esp32/state", &message);
        if (rc != 0) {
            printf("Error publishing message\n");
        } else {
            printf("Message published to topic esp32/state\n");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publisher_thread, NULL, "publisher");

    return 0;
}