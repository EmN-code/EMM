#include <thread.h>
#include <timer.h>
#include <string.h>
#include <stdio.h>
#include <mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/state"
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];

static void *mqtt_thread(void *arg) {
    (void)arg;
    mqtt_client_t client;
    mqtt_connect_info_t connect_info = {
        .client_id = MQTT_CLIENT_ID,
        .will_topic = NULL,
        .will_message = NULL,
        .keep_alive = 10,
        .clean_session = 1,
        .username = NULL,
        .password = NULL
    };

    mqtt_message_t message;
    message.qos = QOS0;
    message.retained = false;
    message.payload = "work";
    message.payload_len = strlen(message.payload);

    while (1) {
        if (mqtt_connect(&client, MQTT_BROKER_IP, MQTT_PORT, &connect_info) == SUCCESS) {
            if (mqtt_publish(&client, MQTT_TOPIC, &message) == SUCCESS) {
                puts("Message Published: work");
            } else {
                puts("Failed to publish message");
            }
            mqtt_disconnect(&client);
        } else {
            puts("Failed to connect to broker");
        }
        ztimer_sleep(ZTIMER_USEC, 5000000);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "MQTT thread");
    return 0;
}