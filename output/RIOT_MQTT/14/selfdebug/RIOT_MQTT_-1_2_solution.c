#include <string.h>
#include "xtimer.h"
#include "net/mqtt.h"
#include "net/ipv6/addr.h"
#include "kernel.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"
#define MESSAGE "work"
#define INTERVAL (5U * US_PER_SEC)

static kernel_pid_t mqtt_thread_pid;
static char mqtt_thread_stack[THREAD_STACKSIZE_DEFAULT];
static bool is_work = true;

static void *mqtt_thread(void *arg) {
    (void)arg;
    while (1) {
        mqtt_publish((mqtt_socket_t *)arg, 0, TOPIC, (uint8_t *)MESSAGE, strlen(MESSAGE), 0);
        is_work = !is_work;
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    mqtt_client client;
    mqtt_connect_params_t connect_params;
    ipv6_addr_t broker_addr;

    // Convert the string representation of the broker's IP address to a byte array
    inet_pton(AF_INET, BROKER_IP, &broker_addr.u8);

    connect_params.client_id = CLIENT_ID;
    connect_params.keep_alive = 60;
    connect_params.clean_session = 1;

    mqtt_init(&client, &connect_params);
    mqtt_connect(&client, &broker_addr, BROKER_PORT);

    // Create an MQTT publish thread
    mqtt_thread_pid = thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack),
                                    THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                    mqtt_thread, &client, "mqtt_thread");

    // Keep the main function running
    thread_sleep();

    return 0;
}