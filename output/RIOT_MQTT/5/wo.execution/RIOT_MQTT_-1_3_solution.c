#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "esp_event.h"
#include "MQTTClient.h"

#define BROKER_IP    "47.102.103.1"
#define BROKER_PORT  1883
#define PUBLISH_INTERVAL_SEC 5

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];
static ztimer_t timer;

void publish_state(void) {
    MQTTMessage message;
    char payload[20];
    
    snprintf(payload, sizeof(payload), "state: %s", "work");
    
    message.qos = QOS1;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);
    
    MQTTPublish(&client, "state", &message);
}

void timer_callback(void *arg) {
    (void)arg;
    publish_state();
    ztimer_set_msg(ZTIMER_SEC, &timer, PUBLISH_INTERVAL_SEC, &timer);
}

int main(void) {
    ztimer_set_msg(ZTIMER_SEC, &timer, PUBLISH_INTERVAL_SEC, &timer);
    
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTConnect(&client, NULL);

    timer.callback = timer_callback;
    ztimer_set(ZTIMER_SEC, &timer, PUBLISH_INTERVAL_SEC);
    
    while (1) {
        /* Here we yield control, allowing other threads to run */
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }

    return 0;
}
