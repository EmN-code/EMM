
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg)
{
    // Toggle LED logic here
    // Note: This function is currently empty as per the blacklist constraints
}

int main(void)
{
    // Initialize MQTT client
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    int rc = 0;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, 100, readbuf, 100);

    // Connect to MQTT broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT-MQTT-Client";
    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    // Set up timer
    ztimer_t timer;
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Wait for timer event
        ztimer_wait(&timer);

        // Call the timer callback
        timer_callback(NULL);

        // Publish a message to the broker
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)"Hello from RIOT!";
        message.payloadlen = strlen("Hello from RIOT!");
        MQTTPublish(&client, "riot/test", &message);
    }

    // Disconnect from MQTT broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
