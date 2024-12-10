#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID CONFIG_BOARD
#define MQTT_PUB_TOPIC "state"  // Topic to publish to
#define MQTT_PAYLOAD "work"
#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct mqtt_client client;
static struct sockaddr_in broker;

void mqtt_evt_handler(struct mqtt_client *const cli, const struct mqtt_evt *evt)
{
	// Handle different MQTT events
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		if (evt->result != 0) {
			printk("MQTT connect failed
");
		}
		break;
	case MQTT_EVT_DISCONNECT:
		printk("MQTT disconnected
");
		break;
	case MQTT_EVT_PUBACK:
		printk("Message published
");
		break;
	default:
		break;
	}
}

void publisher_thread(void)
{
	struct mqtt_publish_param param;
	param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
	param.message.topic.topic.utf8 = (uint8_t *)MQTT_PUB_TOPIC;
	param.message.topic.topic.size = strlen(MQTT_PUB_TOPIC);
	param.message.payload.data = (uint8_t *)MQTT_PAYLOAD;
	param.message.payload.len = strlen(MQTT_PAYLOAD);
	param.message_id = sys_rand32_get();
	
	while (1) {
		mqtt_publish(&client, &param);
		k_sleep(K_SECONDS(5));
	}
}

void main(void)
{
	struct sockaddr_in *broker = (struct sockaddr_in *)&client.broker;
	
	mqtt_client_init(&client);
	
	broker->sin_family = AF_INET;
	broker->sin_port = htons(MQTT_BROKER_PORT);
	inet_pton(AF_INET, MQTT_BROKER_IP, &broker->sin_addr);
	
	client.broker = broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
	client.client_id.size = strlen(MQTT_CLIENT_ID);
	client.protocol_version = MQTT_VERSION_3_1_1;
	client.rx_buf = rx_buffer;
	client.rx_buf_size = sizeof(rx_buffer);
	client.tx_buf = tx_buffer;
	client.tx_buf_size = sizeof(tx_buffer);
	
	if (mqtt_connect(&client) != 0) {
		printk("Failed to connect to MQTT broker
");
		return;
	}
	
	k_thread_start(k_thread_create(&publisher_thread_data,
			publisher_thread_stack, STACK_SIZE,
			(k_thread_entry_t)publisher_thread,
			NULL, NULL, NULL,
			THREAD_PRIORITY, 0, K_NO_WAIT));
}