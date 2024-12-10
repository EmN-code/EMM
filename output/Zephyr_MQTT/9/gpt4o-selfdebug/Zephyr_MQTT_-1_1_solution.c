#include <zephyr/sys/printk.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883

#define MQTT_CLIENTID "zephyr_client"
#define TOPIC "test/topic"
#define PAYLOAD1 "work"
#define PAYLOAD2 "not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;

static void prepare_fds(struct mqtt_client *client)
{
    static struct pollfd fds[1];

    fds[0].fd = client->transport.tcp.sock;
    fds[0].events = POLLIN;

    client->transport.pollin_fd = fds;
    client->transport.pollin_fd_count = ARRAY_SIZE(fds);
}

static int publish(struct mqtt_client *client, enum mqtt_qos qos, const char *payload)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = qos;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = (uint8_t *)payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    return mqtt_publish(client, &param);
}

static void broker_init(void)
{
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDRESS, &broker.sin_addr.s_addr);
}

static void client_init(struct mqtt_client *client)
{
    mqtt_client_init(client);

    broker_init();

    client->broker = &broker;
    client->evt_cb = NULL;  // Can set to event handler if needed
    client->client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client->client_id.size = strlen(MQTT_CLIENTID);
    client->password = NULL;
    client->user_name = NULL;
    client->protocol_version = MQTT_VERSION_3_1_1;

    client->transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void mqtt_thread(void)
{
    int rc;
    struct pollfd fds[1];
    int i = 0;

    client_init(&client);

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("Failed to connect to broker [%d]", rc);
        return;
    }

    prepare_fds(&client);

    while (1) {
        rc = publish(&client, MQTT_QOS_1, (i % 2 == 0) ? PAYLOAD1 : PAYLOAD2);
        if (rc != 0) {
            printk("Failed to publish message [%d]", rc);
            mqtt_disconnect(&client);
        }

        i++;

        rc = poll(fds, ARRAY_SIZE(fds), mqtt_keepalive_time_left(&client));
        if (rc < 0) {
            printk("ERROR: poll handler failure [%d]", rc);
            break;
        }
        mqtt_input(&client);
        mqtt_live(&client);

        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);
}

void main(void)
{
    printk("MQTT simple publisher
");

    k_thread_create(&my_thread_data, my_stack_area, K_THREAD_STACK_SIZEOF(my_stack_area), (k_thread_entry_t)mqtt_thread, NULL, NULL, NULL, MY_PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(&my_thread_data, "mqtt_thread");
}