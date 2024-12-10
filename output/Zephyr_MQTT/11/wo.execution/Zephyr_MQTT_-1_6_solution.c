
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <inet_pton.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

LOG_MODULE_REGISTER(main);

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                LOG_INF("Connected to MQTT broker");
            } else {
                LOG_ERR("Failed to connect to MQTT broker: %d", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("Disconnected from MQTT broker");
            break;
        default:
            break;
    }
}

int main(void)
{
    struct mqtt_client client;
    struct sockaddr_in broker;
    struct mqtt_connect_param connect_params;
    struct mqtt_publish_param publish_params;
    struct mqtt_publish_message publish_message;
    uint8_t tx_buffer[128];
    uint8_t rx_buffer[128];

    memset(&client, 0, sizeof(client));
    memset(&broker, 0, sizeof(broker));
    memset(&connect_params, 0, sizeof(connect_params));
    memset(&publish_params, 0, sizeof(publish_params));
    memset(&publish_message, 0, sizeof(publish_message));

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    connect_params.client_id.utf8 = "zephyr_client";
    connect_params.client_id.size = strlen("zephyr_client");
    connect_params.user_name = NULL;
    connect_params.password = NULL;
    connect_params.clean_session = 1;
    connect_params.keepalive = 60;

    mqtt_client_init(&client);
    mqtt_connect(&client, &connect_params);

    publish_message.topic.utf8 = MQTT_TOPIC;
    publish_message.topic.size = strlen(MQTT_TOPIC);
    publish_message.payload.data = MQTT_MESSAGE;
    publish_message.payload.len = strlen(MQTT_MESSAGE);
    publish_message.qos = MQTT_QOS_0_AT_MOST_ONCE;

    publish_params.message = publish_message;
    publish_params.dup_flag = 0;
    publish_params.retain_flag = 0;

    while (1) {
        mqtt_publish(&client, &publish_params);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
