
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/websocket.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/slist.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/posix/arpa/inet.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work or not work"

struct mqtt_sn_client client;
struct mqtt_sn_transport transport;
struct mqtt_sn_data client_id = {
    .data = MQTT_CLIENT_ID,
    .len = sizeof(MQTT_CLIENT_ID) - 1
};

void mqtt_sn_event_handler(struct mqtt_sn_client *client, struct mqtt_sn_evt *evt)
{
    switch (evt->type) {
        case MQTT_SN_EVT_CONNECTED:
            printk("MQTT-SN connected\n");
            break;
        case MQTT_SN_EVT_DISCONNECTED:
            printk("MQTT-SN disconnected\n");
            break;
        default:
            break;
    }
}

void publish_state(struct mqtt_sn_client *client)
{
    struct mqtt_sn_data topic = {
        .data = MQTT_TOPIC,
        .len = sizeof(MQTT_TOPIC) - 1
    };
    struct mqtt_sn_data message = {
        .data = MQTT_MESSAGE,
        .len = sizeof(MQTT_MESSAGE) - 1
    };

    mqtt_sn_publish(client, MQTT_SN_QOS_0, &topic, false, &message);
}

int main(void)
{
    int sock;
    struct sockaddr_in broker_addr;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to broker\n");
        close(sock);
        return -1;
    }

    transport.type = MQTT_TRANSPORT_NON_SECURE;
    transport.tcp.sock = sock;

    mqtt_sn_client_init(&client, &client_id, &transport, mqtt_sn_event_handler, NULL, 0, NULL, 0);
    mqtt_sn_connect(&client, false, true);

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
