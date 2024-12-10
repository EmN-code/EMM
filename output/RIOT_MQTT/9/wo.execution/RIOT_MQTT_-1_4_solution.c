/* The below program is for the ESP32 to connect to an MQTT broker and publish a message every 5 seconds. */

#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "event/thread.h"
#include "thread.h"
#include "esp_system.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define BUFSIZE 128
#define COMMAND_TIMEOUT_MS 1000

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *thread_handler(void *arg) {
    (void)arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    char state[20];
    Network network;
    MQTTClient client;
    unsigned char sendbuf[BUFSIZE], readbuf[BUFSIZE];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, BUFSIZE, readbuf, BUFSIZE);
    int rc = NetworkConnect(&network, "47.102.103.1", 1883);

    if (rc != 0) {
        printf("Error connecting to broker: %d\n", rc);
        return NULL;
    }

    options.MQTTVersion = 3;
    options.clientID.cstring = "ESP32Client";
    rc = MQTTConnect(&client, &options);

    if (rc != 0) {
        printf("Error connecting to MQTT: %d\n", rc);
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS1;
        message.retained = 0;
        message.payload = (void *)state;
        message.payloadlen = strlen(state);

        snprintf(state, sizeof(state), "work or not work");
        rc = MQTTPublish(&client, "esp32/state", &message);
        if (rc != 0) {
            printf("Error publishing message: %d\n", rc);
        }

        xtimer_periodic_wakeup(&last_wakeup, 5000000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "mqtt_thread");
    return 0;
}

