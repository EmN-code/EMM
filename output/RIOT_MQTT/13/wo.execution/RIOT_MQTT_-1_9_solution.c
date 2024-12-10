#include <ztimer.h>
#include <MQTTClient.h>
#include <stdio.h>
#include <stdint.h>
#include <event/thread.h>
#include <esp_system.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <xtimer.h>
#include <time_units.h>
#include <unistd.h>
#include <mutex.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <stdlib.h>
#include <paho_mqtt.h>
#include <mbox.h>
#include <architecture.h>
#include <msg.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>

#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL (5U * 1000U * 1000U)  // 5 seconds

static char mqtt_stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];

    NetworkConnect(&network, MQTT_BROKER, MQTT_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;

    MQTTConnect(&client, &data);

    MQTTMessage message;
    message.qos = 1;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");

    while (1) {
        MQTTPublish(&client, MQTT_TOPIC, &message);
        sleep(PUBLISH_INTERVAL / 1000 / 1000);  // Convert microseconds to seconds
    }

    return NULL;
}

int main(void) {
    thread_create(mqtt_stack, sizeof(mqtt_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}