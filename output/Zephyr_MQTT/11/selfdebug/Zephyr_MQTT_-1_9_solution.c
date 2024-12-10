
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <logging/log.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

LOG_MODULE_REGISTER(main);

struct mqtt_client client_ctx;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void publish_state(struct mqtt_client *client) {
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };

    param.message.topic = topic;
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = strlen(MQTT_PAYLOAD);
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish message, error: %d", ret);
    } else {
        LOG_INF("Message published successfully");
    }
}

void main(void) {
    struct sockaddr_in broker;
    int ret;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.transport.tcp.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_ctx.transport.tcp.sock < 0) {
        LOG_ERR("Failed to create socket, error: %d", client_ctx.transport.tcp.sock);
        return;
    }

    ret = connect(client_ctx.transport.tcp.sock, (struct sockaddr *)&broker, sizeof(broker));
    if (ret < 0) {
        LOG_ERR("Failed to connect to broker, error: %d", ret);
        close(client_ctx.transport.tcp.sock);
        return;
    }

    client_ctx.client_id.utf8 = "esp32_client";
    client_ctx.client_id.size = strlen("esp32_client");
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", ret);
        close(client_ctx.transport.tcp.sock);
        return;
    }

    while (1) {
        publish_state(&client_ctx);
        k_sleep(K_SECONDS(5));
    }
}
