#include <ztimer.h>
#include <stdbool.h>
#include <unistd.h>
#include <thread.h>
#include <timex.h>
#include <msg.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <stdio.h>

void *mqtt_connection_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    int rc;
    char address[] = "test.mosquitto.org";

    while (1) {
        rc = NetworkConnect(&network, address, 1883);
        if (rc != 0) {
            printf("Network connection failed\n");
            sleep(1);
            continue;
        } else {
            printf("Connected to network\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    // Complete your MQTT setup and subscribe to topics here

    return NULL;
}

static void timer_cb(void *arg) {
    (void)arg;
    // Simulate turning on an LED
    printf("Timer triggered, LED state changed\n");
}

int main(void) {
    kernel_pid_t thread_pid;
    char thread_stack[THREAD_STACKSIZE_MAIN];

    thread_pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                               mqtt_connection_thread, NULL, "mqtt_connection");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return -1;
    }

    /* Initialize timer with 1-second interval */
    tim_t dev = 0;
    timer_init(dev, 1, timer_cb, NULL);
    timer_start(dev);

    while (1) {
        thread_yield();
    }

    return 0;
}
