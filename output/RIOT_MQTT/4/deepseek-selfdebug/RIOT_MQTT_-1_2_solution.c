
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_TIMEOUT_MS 1000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + MQTT_BUFFER_SIZE)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static sock_tcp_t mqtt_sock;
static char mqtt_buffer[MQTT_BUFFER_SIZE];
static char mqtt_read_buffer[MQTT_BUFFER_SIZE];

static void mqtt_connect(void)
{
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    sock_tcp_create(&mqtt_sock, NULL, (struct sockaddr *)&broker_addr, 0);
    sock_tcp_connect(&mqtt_sock, (struct sockaddr *)&broker_addr, SOCK_FLAGS_NONE);

    mqtt_connect_client(&mqtt_sock, MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD,
                        MQTT_KEEPALIVE, MQTT_CLEAN_SESSION, MQTT_WILL_TOPIC,
                        MQTT_WILL_MESSAGE, MQTT_WILL_QOS, MQTT_WILL_RETAIN,
                        mqtt_buffer, MQTT_BUFFER_SIZE, MQTT_TIMEOUT_MS);
}

static void mqtt_publish(const char *topic, const char *message)
{
    mqtt_publish_client(&mqtt_sock, topic, message, strlen(message), MQTT_QOS,
                        MQTT_RETAIN, mqtt_buffer, MQTT_BUFFER_SIZE, MQTT_TIMEOUT_MS);
}

int main(void)
{
    mqtt_connect();

    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
