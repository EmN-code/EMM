#include zephyr/syscalls/socket.h
#include zephyr/syscalls/kernel.h
#include zephyr/sys/device_mmio.h
#include zephyr/sys/mem_stats.h
#include zephyr/sys/slist.h
#include zephyr/sys/math_extras.h
#include zephyr/stats/stats.h
#include zephyr/storage/flash_map.h
#include zephyr/sys/fdtable.h
#include zephyr/syscalls/random.h
#include zephyr/syscalls/flash.h
#include zephyr/syscalls/device.h
#include zephyr/syscalls/i2c.h
#include zephyr/net/socket_select.h
#include zephyr/net/socket_poll.h
#include zephyr/net/dns_resolve.h
#include zephyr/net/ethernet.h
#include zephyr/net/net_ip.h
#include zephyr/net/socket_poll.h
#include zephyr/net/socket.h
#include zephyr/net/offloaded_netdev.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/buf.h
#include zephyr/net/tls_credentials.h
#include zephyr/fs/nvs.h
#include zephyr/tracing/tracing.h
#include zephyr/tracing/tracing_macros.h
#include zephyr/device.h
#include zephyr/random/random.h
#include zephyr/kernel.h
#include zephyr/devicetree.h
#include zephyr/posix/unistd.h
#include zephyr/posix/sys/socket.h
#include zephyr/posix/netdb.h
#include zephyr/posix/arpa/inet.h
#include sys/time.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printf("Connected to broker\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printf("Disconnected from broker\n");
        break;
    case MQTT_EVT_PUBACK:
        printf("Publish acknowledged\n");
        break;
    default:
        break;
    }
}

void connect_and_publish(struct mqtt_client *client) {
    struct sockaddr_in broker;
    broker.sin_family = AF_INET;
    broker.sin_port = htons(1883);
    inet_pton(AF_INET, "47.102.103.1", &broker.sin_addr);

    memset(&client, 0, sizeof(client));
    client->broker = (void *)&broker;
    client->evt_cb = mqtt_event_handler;

    if (mqtt_connect(client) != 0) {
        printf("Failed to connect to broker\n");
        return;
    }

    while (1) {
        struct mqtt_publish_param param;
        memset(&param, 0, sizeof(param));
        param.message.topic.topic.utf8 = (uint8_t *)"device/state";
        param.message.topic.topic.size = strlen("device/state");
        param.message.payload.data = (uint8_t *)"work";
        param.message.payload.len = strlen("work");

        if (mqtt_publish(client, &param) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        sleep(5);
    }
}

int main() {
    struct mqtt_client client;
    connect_and_publish(&client);
    return 0;
}