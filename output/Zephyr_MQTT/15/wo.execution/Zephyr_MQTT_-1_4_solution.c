
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <string.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_topic topic;
struct mqtt_publish_param publish_params;
struct mqtt_publish_message publish_msg;

void mqtt_connect_and_publish(void) {
    int sock;
    struct sockaddr_in broker_addr;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    client.socket = sock;
    mqtt_client_init(&client);

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS_0_AT_MOST_ONCE;

    publish_msg.topic = &topic;
    publish_msg.payload.data = MQTT_PAYLOAD;
    publish_msg.payload.len = strlen(MQTT_PAYLOAD);

    publish_params.message = publish_msg;
    publish_params.message_id = 0;
    publish_params.dup_flag = 0;
    publish_params.retain_flag = 0;

    if (mqtt_publish(&client, &publish_params) != 0) {
        printk("Failed to publish message\n");
    } else {
        printk("Message published successfully\n");
    }

    close(sock);
}

int main(void) {
    while (1) {
        mqtt_connect_and_publish();
        k_sleep(K_SECONDS(5));
    }
    return 0;
}
