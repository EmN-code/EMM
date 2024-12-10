#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>
#include <net/net_ip.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "ESP32_MQTT_Client"
#define MQTT_TOPIC "device/state"

static struct mqtt_client client;
static struct sockaddr_storage broker;

void prepare_fds(struct mqtt_sec_config *tls_config, struct mqtt_client *client)
{
	/* Setup socket structures here */
	struct sockaddr_in *broker4 = ((struct sockaddr_in *)&broker);
	
	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(BROKER_PORT);
	inet_pton(AF_INET, BROKER_ADDR, &broker4->sin_addr);
}

static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
	switch (evt->type) {
	case MQTT_EVT_CONNACK:
		printf("MQTT client connected!");
		break;
	default:
		break;
	}
}

void configure_mqtt(void)
{
	mqtt_client_init(&client);
	
	client.broker = &broker;
	client.evt_cb = mqtt_evt_handler;
	client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
	client.client_id.size = strlen(MQTT_CLIENTID);
	arbitrary unique identifier for the client
}

void publish_state(void)
{
	struct mqtt_publish_param param;
	param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
	param.message.topic.topic.size = strlen(MQTT_TOPIC);
	param.message.payload.data = (uint8_t *)"work";
	param.message.payload.len = 4;
	param.message_id = sys_rand32_get();
	param.dup_flag = 0;
	param.retain_flag = 0;
	param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
	
	mqtt_publish(&client, &param);
}

void main(void)
{
	configure_mqtt();
	
	prepare_fds(NULL, &client);
	
	mqtt_connect(&client);
	
	while (1) {
		if (mqtt_live(&client) < 0) {
			break;
		}
		publish_state();
		k_sleep(K_SECONDS(5));
	}

	mqtt_disconnect(&client);
}