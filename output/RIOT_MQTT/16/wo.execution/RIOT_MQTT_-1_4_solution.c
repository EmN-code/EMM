#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5 /* publish every 5 seconds */

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

static void publish_state(void) {
    MQTTMessage message;
    char *payload = "work or not work";

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(&client, "state", &message);
}

void *mqtt_thread(void *arg) {
    (void)arg;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    MQTTConnect(&client, &connectData);

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");
    return 0;
}