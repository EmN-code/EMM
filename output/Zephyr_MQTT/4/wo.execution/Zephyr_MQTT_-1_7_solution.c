
#include <zephyr/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/socket.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/pm/state.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/net_if.h>
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
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0

struct mqtt_sn_client client;

void mqtt_sn_event_handler(struct mqtt_sn_client *client, struct mqtt_sn_evt *evt)
{
    switch (evt->type) {
        case MQTT_SN_EVT_CONNECTED:
            printk("MQTT-SN connected to broker\n");
            break;
        case MQTT_SN_EVT_DISCONNECTED:
            printk("MQTT-SN disconnected from broker\n");
            break;
        default:
            break;
    }
}

int connect_to_broker(void)
{
    struct sockaddr_in broker_addr;
    int sock;

    sock = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    if (zsock_connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to broker\n");
        zsock_close(sock);
        return -1;
    }

    return sock;
}

void publish_state(struct mqtt_sn_client *client)
{
    struct mqtt_sn_publish_param param;
    struct mqtt_sn_topic topic;
    struct mqtt_sn_binstr payload;

    topic.name = MQTT_TOPIC;
    topic.name_len = strlen(MQTT_TOPIC);

    payload.data = MQTT_PAYLOAD;
    payload.size = MQTT_PAYLOAD_SIZE;

    param.topic = &topic;
    param.payload = &payload;
    param.qos = MQTT_QOS;
    param.retain = 0;

    mqtt_sn_publish(client, &param);
}

int main(void)
{
    int sock;

    sock = connect_to_broker();
    if (sock < 0) {
        return -1;
    }

    mqtt_sn_client_init(&client);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.keepalive = MQTT_KEEPALIVE;
    client.evt_cb = mqtt_sn_event_handler;

    if (mqtt_sn_connect(&client, false, true) != 0) {
        printk("Failed to connect to MQTT-SN broker\n");
        zsock_close(sock);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    mqtt_sn_disconnect(&client);
    zsock_close(sock);

    return 0;
}
