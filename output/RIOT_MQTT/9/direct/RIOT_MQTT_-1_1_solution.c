#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "net/mqtt.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/gnrc.h"
#include "random.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID        "riot-esp32"
#define TOPIC_NAME       "device/state"
#define INTERVAL         (5U)

static char mqtt_pub_stack[THREAD_STACKSIZE_DEFAULT];

void *mqtt_pub_thread(void *arg) {
    (void)arg;
    mqtt_t client;
    mqtt_opt_t opts;
    opts.hostname = MQTT_BROKER_ADDR;
    opts.port = MQTT_BROKER_PORT;
    opts.clientid = CLIENT_ID;
    mqtt_init(&client, &opts);

    client.connect(&client);

    while (1) {
        char payload[32];
        int state = random_uint32_range(0, 2); // Random state for "work" or "not work"
        if (state) {
            snprintf(payload, sizeof(payload), "work");
        } else {
            snprintf(payload, sizeof(payload), "not work");
        }

        printf("Publishing state: %s\n", payload);
        mqtt_publish(&client, false, TOPIC_NAME, payload, strlen(payload));

        sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("MQTT client example for ESP32");
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(mqtt_pub_stack, sizeof(mqtt_pub_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKSIZE_DEFAULT, mqtt_pub_thread, NULL, "mqtt_pub_thread");

    return 0;
}