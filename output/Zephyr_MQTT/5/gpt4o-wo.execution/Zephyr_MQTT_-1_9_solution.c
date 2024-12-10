#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/init.h>
#include <zephyr/sys/printk.h>
#include <zephyr/stats/stats.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/syscalls/i2c.h>
#include <sys/time.h>
#include <time.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;

void prepare_fds(struct mqtt_client *c, struct pollfd *fds)
{
    fds->fd = c->transport.tcp.sock;
    fds->events = POLLIN;

    if (c->transport.tcp.sock != INVALID_SOCKET) {
        fds->revents = 0U;
    }
}

void publish_state(struct mqtt_client *c)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = "/state";
    param.message.topic.topic.size = strlen(param.message.topic.topic.utf8);
    param.message.payload.data = "work or not work";
    param.message.payload.len = strlen(param.message.payload.data);

    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    mqtt_publish(c, &param);
}

void configure_mqtt_client(struct mqtt_client *client)
{
    mqtt_client_init(client);

    client->broker = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
    };
    inet_pton(AF_INET, SERVER_ADDR, &client->broker.sin_addr);
    client->mqtt_version = MQTT_VERSION_3_1_1;
    client->transport.type = MQTT_TRANSPORT_NON_SECURE;
}

int main(void)
{
    struct pollfd fds;
    int err;

    printk("Connecting to MQTT broker at %s:%d\n", SERVER_ADDR, SERVER_PORT);

    configure_mqtt_client(&client);

    err = mqtt_connect(&client);
    if (err) {
        printk("Failed to connect, error %d\n", err);
        return -1;
    }

    while (1) {
        prepare_fds(&client, &fds);

        if (poll(&fds, 1, mqtt_keepalive_time_left(&client))) {
            mqtt_input(&client);
        } else {
            mqtt_ping(&client);
        }

        publish_state(&client);
        k_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}