#include <thread.h>
#include <stdio.h>
#include <mqtt-sn.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5 * US_PER_SEC)  // Publish every 5 seconds

static char my_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t my_msg_queue[8];

static void *mqtt_thread(void *arg) {
    (void)arg;
    mqtt_sn_t mqtt_client;
    mqtt_sn_config_t config = {
        .brokers = BROKER_IP,
        .port = BROKER_PORT,
    };
    mqtt_sn_init(&mqtt_client, &config);
    mqtt_sn_connect(&mqtt_client);

    while (1) {
        printf("Publishing message...\n");
        mqtt_sn_publish(&mqtt_client, "state", "work or not work", strlen("work or not work"), 0);
        xtimer_sleep(PUB_INTERVAL);
    }

    mqtt_sn_disconnect(&mqtt_client);

    return NULL;
}

int main(void) {
    puts("MQTT example on RIOT
");

    thread_create(my_stack, sizeof(my_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_sn");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}