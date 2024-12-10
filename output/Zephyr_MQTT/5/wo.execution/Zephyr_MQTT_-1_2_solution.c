
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <net/net_mgmt.h>
#include <net/wifi_mgmt.h>
#include <storage/flash_map.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

#define STACK_SIZE 1024
#define PRIORITY 7

K_THREAD_STACK_DEFINE(thread_stack, STACK_SIZE);

void mqtt_publish_thread(void) {
    int sock;
    struct sockaddr_in broker_addr;
    struct mqtt_client client;
    uint8_t sendbuf[128];
    uint8_t recvbuf[128];
    struct mqtt_publish_param param;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    mqtt_client_init(&client);
    client.broker = &broker_addr;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.sendbuf = sendbuf;
    client.sendbuf_size = sizeof(sendbuf);
    client.recvbuf = recvbuf;
    client.recvbuf_size = sizeof(recvbuf);

    if (mqtt_connect(&client) != 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.utf8 = (uint8_t *)MQTT_MESSAGE;
    param.message.payload.size = strlen(MQTT_MESSAGE);
    param.message_id = 1U;
    param.dup_flag = 0U;
    param.retain_flag = 0U;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    while (1) {
        if (mqtt_publish(&client, &param) != 0) {
            printk("Failed to publish message\n");
        } else {
            printk("Message published\n");
        }
        k_sleep(K_SECONDS(5));
    }
}

int main(void) {
    k_thread_create(&mqtt_publish_thread, thread_stack, STACK_SIZE,
                    (k_thread_entry_t)mqtt_publish_thread, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
    return 0;
}
