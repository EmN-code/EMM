#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/protnum.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U)  // 5 seconds

static const char *topic = "state";
static char *payload = "work or not work";
static kernel_pid_t mqtt_thread_pid;

static void *mqtt_thread(void *arg) {
    (void)arg;
    // Initialize the network socket
    sock_udp_ep_t broker_ep = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, BROKER_IP, &broker_ep.addr.ipv6);

    MQTTClient client;
    Network network;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    // Connect to the MQTT broker
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Network connect failed\n");
        return NULL;
    }
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connect failed\n");
        NetworkDisconnect(&network);
        return NULL;
    }

    // Main loop to publish message
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        MQTTPublish(&client, topic, &message);

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL * MS_PER_SEC);
    }

    // Disconnect
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return NULL;
}

int main(void) {
    mqtt_thread_pid = thread_create(mqtt_thread_pid_stack, sizeof(mqtt_thread_pid_stack),
                        THREAD_PRIORITY_MAIN - 1, 0,
                        mqtt_thread, NULL, "mqtt");
    if (mqtt_thread_pid <= KERNEL_PID_UNDEF) {
        puts("error initializing mqtt thread\n");
        return 1;    
    }
    return 0;
}