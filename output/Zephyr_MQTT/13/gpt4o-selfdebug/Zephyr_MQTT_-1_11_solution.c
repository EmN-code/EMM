#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/net/socket.h>
#include <zephyr/device.h>

#define STACKSIZE 1024
#define PRIORITY 7

void mqtt_publish(struct mqtt_client *client) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "esp32/state";
    param.message.topic.topic.size = strlen("esp32/state");

    param.message.payload.data = (void *)"work";
    param.message.payload.len = strlen("work");

    param.retain_flag = 0;
    param.dup_flag = 0;
    param.message_id = mqtt_next_message_id_get(client);

    mqtt_publish(client, &param);
}

void main() {
    struct sockaddr_in broker;
    struct mqtt_client client;
    uint8_t rx_buffer[128];
    uint8_t tx_buffer[128];

    broker.sin_family = AF_INET;
    broker.sin_port = htons(1883);
    zsock_inet_pton(AF_INET, "47.102.103.1", &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);
    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        mqtt_publish(&client);
        k_sleep(K_SECONDS(5));
    }
}