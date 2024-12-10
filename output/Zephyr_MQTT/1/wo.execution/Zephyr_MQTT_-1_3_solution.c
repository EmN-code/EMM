
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

void mqtt_publish(int sock, const char *topic, const char *message) {
    struct mqtt_publish_param param;
    param.message.topic.topic.utf8 = topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = message;
    param.message.payload.len = strlen(message);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    if (mqtt_publish(sock, &param) < 0) {
        printk("Failed to publish message\n");
    } else {
        printk("Message published successfully\n");
    }
}

int main(void) {
    int sock;
    struct sockaddr_in broker;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return -1;
    }

    while (1) {
        mqtt_publish(sock, MQTT_TOPIC, MQTT_MESSAGE);
        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}
