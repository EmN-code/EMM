#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <time_units.h>

// Function to configure the MQTT connection
void configure_mqtt_connection(Network* network) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, "MQTT_BROKER_IP", MQTT_BROKER_PORT);
        if(rc != 0) {
            printf("MQTT network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("MQTT network connected successfully.\n");
            break;
        }
    }
}

// Callback function for the timer event
void on_timer_event(void *arg) {
    (void)arg; // Unused variable
    printf("Timer event triggered. LED should toggle here.\n");
    // Here you would typically toggle a LED
}

int main(void) {
    ztimer_t timer;
    Network network;

    // Configure the MQTT connection, retry until successful
    configure_mqtt_connection(&network);

    // Initialize the timer to trigger every second
    ztimer_set(&timer, ZTIMER_SEC, 1, on_timer_event, NULL);

    while (1) {
        thread_yield(); // Yield to allow other threads to run
    }

    return 0;
}