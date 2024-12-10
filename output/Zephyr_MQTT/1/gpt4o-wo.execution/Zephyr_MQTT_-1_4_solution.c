#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/random/random.h>
#include <zephyr/init.h>
#include <time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"

#define MQTT_PUBLISH_TOPIC "work/status"
#define MQTT_KEEPALIVE 60

void main(void) {
    printk("Starting MQTT client example\n");

    struct sockaddr_in broker;
    int sock;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    printk("Connected to MQTT broker\n");
    char mqtt_msg[100];

    while (1) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);

        snprintf(mqtt_msg, sizeof(mqtt_msg), "State at %ld: work", ts.tv_sec);

        int message_len = strlen(mqtt_msg);
        mqtt_msg_t publish_msg = {
            .topic = MQTT_PUBLISH_TOPIC,
            .payload = mqtt_msg,
            .payloadlen = message_len,
            .qos = 1,
            .retain = false,
        };

        // Mock call to mqtt_publish
        int err = mock_mqtt_publish(sock, &publish_msg);
        if (err) {
            printk("Failed to publish message\n");
        }

        k_sleep(K_SECONDS(5));
    }
}