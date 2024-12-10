#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/poll.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/stats/stats.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <sys/time.h>
#include <time.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID   "esp32-client"

#define MQTT_TOPIC "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

void main(void) {
    struct sockaddr_in broker;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printk("Failed to connect to broker\n");
        return;
    }

    struct mqtt_client client;
    struct mqtt_publish_param param;

    mqtt_client_init(&client);

    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);

    while (1) {
        /* Publish "work" state */
        param.message.topic.qos = MQTT_QOS_1;
        param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = (uint8_t *)MSG_WORK;
        param.message.payload.len = strlen(MSG_WORK);

        mqtt_publish(&client, &param);
        printk("Published: %s\n", MSG_WORK);

        k_sleep(K_SECONDS(5));
    }
}