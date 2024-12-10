
#include <zephyr/devicetree.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/posix/sys/socket.h>

#include <stdio.h>
#include <string.h>
#include <net/socket.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_msg;

void mqtt_connect_and_publish(void) {
    int sock;
    struct sockaddr_in broker;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printf("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    mqtt_client_init(&client);
    client.socket = sock;

    struct mqtt_connect_param connect_param;
    connect_param.keep_alive = 60;
    connect_param.clean_session = 1;
    connect_param.client_id.utf8 = "zephyr_client";
    connect_param.client_id.size = strlen("zephyr_client");

    if (mqtt_connect(&client, &connect_param) != 0) {
        printf("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    publish_msg.topic.utf8 = MQTT_TOPIC;
    publish_msg.topic.size = strlen(MQTT_TOPIC);
    publish_msg.payload.utf8 = MQTT_PAYLOAD;
    publish_msg.payload.size = strlen(MQTT_PAYLOAD);
    publish_msg.qos = MQTT_QOS;
    publish_msg.retain = MQTT_RETAIN;

    publish_param.message = publish_msg;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    if (mqtt_publish(&client, &publish_param) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published successfully\n");
    }

    mqtt_disconnect(&client);
    close(sock);
}

int main(void) {
    while (1) {
        mqtt_connect_and_publish();
        k_sleep(K_SECONDS(5));
    }
    return 0;
}
