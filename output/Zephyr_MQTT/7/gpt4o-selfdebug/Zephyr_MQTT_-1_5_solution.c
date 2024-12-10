#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr.h>
#include <stdio.h>
#include <string.h>

#include <mqtt.h>

#define STACK_SIZE 1024
#define PRIORITY 7

static struct mqtt_client client;
static struct sockaddr broker;
static u8_t rx_buffer[128];
static u8_t tx_buffer[128];
static struct zsock_pollfd fds[1];

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printf("MQTT client connected!");
            break;
        case MQTT_EVT_PUBACK:
            printf("Publish acknowledged.");
            break;
        default:
            break;
    }
}

void mqtt_publish_state(void)
{
    struct mqtt_publish_param param;
    param.message.payload.data = "work";
    param.message.payload.len = strlen("work");
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "state";
    param.message.topic.topic.size = strlen("state");
    param.retain_flag = 0;

    mqtt_publish(&client, &param);
}

void mqtt_thread(void)
{
    broker.sa_family = AF_INET;
    net_sin(&broker)->sin_port = htons(1883);
    net_sin(&broker)->sin_addr.s_addr = inet_addr("47.102.103.1");

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = "zephyr_publisher";
    client.client_id.size = strlen("zephyr_publisher");
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);

    while (1) {
        mqtt_publish_state();
        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    k_thread_create(K_THREAD_DEFINE(mqtt_thread, STACK_SIZE, mqtt_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT));
    return 0;
}