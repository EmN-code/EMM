
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(main);

struct mqtt_client client_ctx;
K_THREAD_STACK_DEFINE(mqtt_thread_stack, 1024);

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

void mqtt_publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = topic;
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.retained = 0;
    param.dup = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message: %d", ret);
    } else {
        LOG_INF("Message published");
    }
}

void mqtt_thread(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return;
    }

    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    int ret = connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
    if (ret < 0) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        close(sock);
        return;
    }

    uint8_t rx_buffer[100];
    uint8_t tx_buffer[100];

    mqtt_client_init(&client_ctx);
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.transport.tcp.sock = sock;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.evt_cb = mqtt_evt_handler;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        close(sock);
        return;
    }

    while (1) {
        mqtt_publish_state(&client_ctx);
        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    k_thread_create(&mqtt_thread_data, mqtt_thread_stack,
                    K_THREAD_STACK_SIZEOF(mqtt_thread_stack),
                    (k_thread_entry_t)mqtt_thread,
                    NULL, NULL, NULL,
                    K_PRIO_PREEMPT(7), 0, K_NO_WAIT);

    return 0;
}
