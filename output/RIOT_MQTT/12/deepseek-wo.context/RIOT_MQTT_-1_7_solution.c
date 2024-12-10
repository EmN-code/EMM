
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS QOS_0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS QOS_0
#define MQTT_WILL_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_VERSION MQTT_VERSION_3_1_1
#define MQTT_TIMEOUT 10000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + MQTT_BUFFER_SIZE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_NAME "mqtt_thread"
#define MQTT_THREAD_FLAGS 0
#define MQTT_THREAD_AUTORELOAD 1
#define MQTT_THREAD_INTERVAL 5000000

static char mqtt_thread_stack[MQTT_THREAD_STACKSIZE];
static msg_t mqtt_thread_msg_queue[8];
static mqtt_client_t mqtt_client;
static uint8_t mqtt_buffer[MQTT_BUFFER_SIZE];

static void mqtt_thread(void *arg)
{
    (void)arg;

    mqtt_client_t *client = &mqtt_client;
    mqtt_connect_info_t connect_info = {
        .client_id = MQTT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .username = MQTT_USERNAME,
        .password = MQTT_PASSWORD,
        .will_topic = MQTT_WILL_TOPIC,
        .will_message = MQTT_WILL_MESSAGE,
        .will_qos = MQTT_WILL_QOS,
        .will_retain = MQTT_WILL_RETAIN,
        .clean_session = MQTT_CLEAN_SESSION,
        .version = MQTT_VERSION
    };

    mqtt_client_init(client);
    mqtt_client_connect(client, MQTT_BROKER_IP, MQTT_BROKER_PORT, &connect_info);

    while (1) {
        mqtt_client_publish(client, MQTT_TOPIC, MQTT_PAYLOAD, sizeof(MQTT_PAYLOAD) - 1, MQTT_QOS, MQTT_RETAIN, MQTT_DUP);
        xtimer_usleep(MQTT_THREAD_INTERVAL);
    }
}

int main(void)
{
    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), MQTT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, MQTT_THREAD_NAME);

    return 0;
}
