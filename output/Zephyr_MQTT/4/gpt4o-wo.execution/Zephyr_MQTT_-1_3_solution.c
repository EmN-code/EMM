#include <zephyr/sys/util.h>
#include <zephyr/posix/poll.h>
#include <zephyr/net/socket.h>
#include <zephyr/device.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/slist.h>
#include <zephyr/types.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <sys/time.h>
#include <zephyr/net/buf.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/pm/state.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/coap.h>
#include <time.h>
#include <zephyr/net/wifi.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/fs/nvs.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5000  // 5000 milliseconds

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct pollfd fds[1];

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT connected\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT disconnected\n");
        break;
    case MQTT_EVT_PUBACK:
        printk("MQTT message published\n");
        break;
    default:
        break;
    }
}

int main(void)
{
    k_sleep(K_SECONDS(2)); // Allow network to initialize

    memset(&broker, 0, sizeof(struct sockaddr_in));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);
    
    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = "zephyr_mqtt";
    client.client_id.size = strlen(client.client_id.utf8);
    client.protocol_version = MQTT_VERSION_3_1_1;
    
    mqtt_connect(&client);

    fds[0].fd = client.transport.tcp.sock;
    fds[0].events = POLLIN;

    while (true) {
        mqtt_input(&client);
        mqtt_live(&client);

        poll(fds, 1, PUBLISH_INTERVAL);

        struct mqtt_publish_param pub_param;
        pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        pub_param.message.payload.data = "work";
        pub_param.message.payload.len = strlen(pub_param.message.payload.data);
        pub_param.message.topic.topic.utf8 = "state";
        pub_param.message.topic.topic.size = strlen(pub_param.message.topic.topic.utf8);

        mqtt_publish(&client, &pub_param);
    }

    return 0;
}