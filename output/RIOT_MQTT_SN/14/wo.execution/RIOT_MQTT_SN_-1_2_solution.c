#include "net/emcute.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/pm.h"
#include "net/gnrc.h"

#define MQTT_BROKER_IP   "47.102.103.1"  // MQTT-SN Broker IP
#define MQTT_PORT        1883             // MQTT-SN Broker Port
#define CLIENT_ID        "esp32client"   // MQTT-SN Client ID
#define EMCUTE_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define STATUS_TOPIC     "device/status" // Topic to publish status

static char stack[THREAD_STACKSIZE_DEFAULT];

// RTC used for managing time intervals
rtc_time_t last_time, curr_time;

static void *emcute_thread(void *args) {
    (void)args;
    emcute_run(MQTT_PORT, CLIENT_ID);
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    while (1) {
        rtc_get_time(&last_time);

        // Logic to switch status between "work" and "not work"
        static bool work_status = true;
        const char *status = work_status ? "work" : "not work";
        emcute_topic_t t;

        // Publishing the status at regular intervals
        t.name = STATUS_TOPIC;
        t.id = 0;  // topic id is usually resolved by the broker

        emcute_req_t pubreq;
        pubreq.flags = EMCUTE_QOS_0;
        pubreq.topic = &t;
        pubreq.data.ptr = status;
        pubreq.data.len = strlen(status);

        if (emcute_connect(&_mqtt_con, MQTT_BROKER_IP, MQTT_PORT, CLIENT_ID) == EMCUTE_OK) {
            emcute_publish(&pubreq);
            emcute_disconnect(&_mqtt_con);
        }

        // Toggle working status for next cycle
        work_status = !work_status;

        // Sleep for 5 seconds
        do {
            rtc_get_time(&curr_time);
        } while ((curr_time.seconds - last_time.seconds) < 5);
    }

    return 0;
}