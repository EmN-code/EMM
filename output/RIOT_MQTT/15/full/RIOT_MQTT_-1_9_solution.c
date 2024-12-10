#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <msg.h>
#include <mutex.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <kernel_defines.h>
#include <assert.h>
#include <paho_mqtt.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static char mqtt_send_buf[256];
static char mqtt_read_buf[256];

static MQTTClient client;
static Network network;

void mqtt_publish_task(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "MQTT_RIOT_Client";

    NetworkInit(&network);
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc == 0) {
            break;
        }
        printf("Network connection failed. Retrying in 1 second...\n");
        xtimer_usleep(1000000);
    }

    MQTTClientInit(&client, &network, 30000, mqtt_send_buf, sizeof(mqtt_send_buf), mqtt_read_buf, sizeof(mqtt_read_buf));

    while (1) {
        MQTTPublish(&client, MQTT_TOPIC, &message);
        printf("State published to topic %s\n", MQTT_TOPIC);
        xtimer_usleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    puts("MQTT client example started");
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, mqtt_publish_task, NULL, "mqtt_publish_task");
    return 0;
}