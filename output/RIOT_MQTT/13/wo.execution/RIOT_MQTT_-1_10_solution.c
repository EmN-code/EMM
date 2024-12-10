#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "architecture.h"
#include "net/emcute.h"
#include "unistd.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define MQTT_SERVER_IP     "47.102.103.1"
#define MQTT_SERVER_PORT   1883
#define MQTT_PUB_TOPIC     "esp32/state"
#define MQTT_PAYLOAD       "not work"
#define MQTT_PAYLOAD_ALT   "work"

static char stack[THREAD_STACKSIZE_DEFAULT];

int main(void) {

    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc = 0;

    NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return -1;
    }

    while(1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)MQTT_PAYLOAD;
        message.payloadlen = strlen(MQTT_PAYLOAD);

        MQTTPublish(&client, MQTT_PUB_TOPIC, &message);

        // Switch payload for next message
        if (strcmp((char *)message.payload, MQTT_PAYLOAD) == 0) {
            message.payload = (void *)MQTT_PAYLOAD_ALT;
            message.payloadlen = strlen(MQTT_PAYLOAD_ALT);
        } else {
            message.payload = (void *)MQTT_PAYLOAD;
            message.payloadlen = strlen(MQTT_PAYLOAD);
        }

        xtimer_sleep(5);
    }

    return 0;
}