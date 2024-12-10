#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "freertos/task.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL 5 /* in seconds */

static void *mqtt_pub_thread(void *arg) {
    (void)arg;
    char payload[20];
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    /* Initialize network and MQTT client */
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    MQTTConnect(&client, &connectData);

    message.qos = 1;
    message.retained = 0;
    message.payload = payload;

    while(1) {
        strcpy(payload, "state: work");
        message.payloadlen = strlen(payload);
        MQTTPublish(&client, "state_topic", &message);

        /* Pause for interval */
        ztimer_sleep(ZTIMER_MSEC, MQTT_PUBLISH_INTERVAL * 1000);
    }

    return NULL;
}

int main(void) {
    /* Create MQTT thread */
    task_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_pub_thread, NULL, "mqtt_pub_thread");
    return 0;
}