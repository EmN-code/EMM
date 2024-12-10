#include zephyr/types.h
#include zephyr/tracing/tracing.h
#include zephyr/tracing/tracing_macros.h
#include zephyr/storage/flash_map.h
#include zephyr/stats/stats.h
#include zephyr/sys/util.h
#include zephyr/sys/slist.h
#include zephyr/sys/printk.h
#include zephyr/sys/mutex.h
#include zephyr/sys/mem_stats.h
#include zephyr/sys/fdtable.h
#include zephyr/sys/device_mmio.h
#include zephyr/pm/state.h
#include zephyr/posix/unistd.h
#include zephyr/posix/sys/socket.h
#include zephyr/posix/poll.h
#include zephyr/posix/arpa/inet.h
#include zephyr/net/wifi_mgmt.h
#include zephyr/net/wifi.h
#include zephyr/net/websocket.h
#include zephyr/net/tls_credentials.h
#include zephyr/net/socket_select.h
#include zephyr/net/socket_poll.h
#include zephyr/net/socket.h
#include zephyr/net/offloaded_netdev.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/net_ip.h
#include zephyr/net/net_if.h
#include zephyr/net/mqtt_sn.h
#include zephyr/net/mqtt.h
#include zephyr/net/ethernet.h
#include zephyr/net/dns_resolve.h
#include zephyr/net/coap.h
#include zephyr/net/buf.h
#include zephyr/kernel_includes.h
#include zephyr/kernel.h

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)
#define CLIENT_ID "my_mqtt_client"
#define TOPIC "device/state"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static char device_state[50];

void prepare_fds(struct mqtt_client *client, struct pollfd *fds)
{
    fds->fd = client->transport.tcp.sock;
    fds->events = POLLIN;
}

void publish_message(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    uint8_t qos = MQTT_QOS_1_AT_LEAST_ONCE;

    snprintf(device_state, sizeof(device_state), "work");
    param.message.topic.qos = qos;
    param.message.topic.topic.utf8 = TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = device_state;
    param.message.payload.len = strlen(device_state);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    mqtt_publish(client, &param);
}

void connect_to_broker(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);

    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (const uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);
}

void main(void)
{
    connect_to_broker();

    struct pollfd fds;
    int rc;

    for (;;) {
        prepare_fds(&client, &fds);

        rc = poll(&fds, 1, PUBLISH_INTERVAL);
        if (rc > 0 && (fds.revents & POLLIN) != 0) {
            mqtt_input(&client);
        }

        mqtt_live(&client);
        publish_message(&client);
    }
}