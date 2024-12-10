
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <time.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/random/random.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/devicetree.h>
#include <zephyr/net/coap.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/sys/device_mmio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

struct mqtt_client client;
struct mqtt_publish_param publish_param;
struct mqtt_publish_message publish_message;
struct mqtt_topic topic;
struct mqtt_binstr payload;

void wifi_connect(void) {
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = "your_wifi_ssid",
        .ssid_length = strlen("your_wifi_ssid"),
        .psk = "your_wifi_password",
        .psk_length = strlen("your_wifi_password"),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        printk("WiFi connection failed\n");
    } else {
        printk("WiFi connected\n");
    }
}

void mqtt_connect(void) {
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
        .sin_addr.s_addr = inet_addr(MQTT_BROKER_IP),
    };

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(sock);
        return;
    }

    client.broker = &broker_addr;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;

    if (mqtt_connect(&client) != 0) {
        printk("MQTT connect failed\n");
        close(sock);
        return;
    }

    printk("MQTT connected\n");
}

void mqtt_publish(void) {
    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    payload.binstr.utf8 = MQTT_PAYLOAD;
    payload.binstr.size = strlen(MQTT_PAYLOAD);

    publish_message.topic = topic;
    publish_message.payload = payload;

    publish_param.message = publish_message;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    if (mqtt_publish(&client, &publish_param) != 0) {
        printk("MQTT publish failed\n");
    } else {
        printk("MQTT published\n");
    }
}

int main(void) {
    wifi_connect();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
