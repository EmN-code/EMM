#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(net_mqtt_publisher_sample, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/random/random.h>

// wifi information
#define WIFI_SSID "EmNets-301"
#define WIFI_PSK "eagle402"

static int connected;
static struct net_mgmt_event_callback wifi_shell_mgmt_cb;


// mqtt config
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_CLIENTID "esp32_test"
#define APP_MQTT_BUFFER_SIZE 128
#define APP_CONNECT_TIMEOUT_MS 2000
#define APP_SLEEP_MSECS 500
#define APP_CONNECT_TRIES 10
#define DEFAULT_KEEPALIVE_SEC 10
#define USERNAME "esp32"
#define PASSWORD "esp32"
#define TOPIC "v1/devices/me/telemetry"
#define PAYLOAD "{\"temperature\": 22.7}"

static struct mqtt_utf8 username = {
	.utf8 = USERNAME,
	.size = strlen(USERNAME)
};
static struct mqtt_utf8 password = {
	.utf8 = PASSWORD,
	.size = strlen(PASSWORD)
};

// buffer for MQTT client
static uint8_t rx_buffer[APP_MQTT_BUFFER_SIZE];
static uint8_t tx_buffer[APP_MQTT_BUFFER_SIZE];
// MQTT client struct
static struct mqtt_client client_ctx;
// MQTT Broker details
static struct sockaddr_storage broker;
// MQTT connected flag
static bool mqtt_connected;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *)
					   cb->info;

	if (status->status) {
		LOG_ERR("Connection request failed (%d)\n", status->status);
	} else {
		LOG_INF("WIFI Connected\n");
		connected = 1;
	}
}

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
				    uint32_t mgmt_event, struct net_if *iface)
{
	switch (mgmt_event) {
	case NET_EVENT_WIFI_CONNECT_RESULT:
		handle_wifi_connect_result(cb);
		break;
	default:
		break;
	}
}

void wifi_connect(void)
{
	int nr_tries = 10;
	int ret = 0;

	net_mgmt_init_event_callback(&wifi_shell_mgmt_cb,
				     wifi_mgmt_event_handler,
				     NET_EVENT_WIFI_CONNECT_RESULT);

	net_mgmt_add_event_callback(&wifi_shell_mgmt_cb);

	struct net_if *iface = net_if_get_default();
	static struct wifi_connect_req_params cnx_params = {
		.ssid = WIFI_SSID,
		.ssid_length = strlen(WIFI_SSID),
		.psk = WIFI_PSK,
		.psk_length = strlen(WIFI_PSK),
		.channel = WIFI_CHANNEL_ANY,
		.security = WIFI_SECURITY_TYPE_PSK,
		.band = WIFI_FREQ_BAND_2_4_GHZ,
	};

	connected = 0;

	LOG_INF("WIFI try connecting to %s...\n", WIFI_SSID);

	/* Let's wait few seconds to allow wifi device be on-line */
	while (nr_tries-- > 0) {
		ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &cnx_params,
			       sizeof(struct wifi_connect_req_params));
		if (ret == 0) {
			break;
		}

		LOG_INF("Connect request failed %d. Waiting iface be up...\n", ret);
		k_msleep(500);
	}

	while (connected == 0) {
		k_msleep(100);
	}
}


void mqtt_evt_handler(struct mqtt_client *const client,const struct mqtt_evt *evt){
	switch(evt->type){
		case MQTT_EVT_CONNACK:
			if (evt->result != 0){
				LOG_ERR("MQTT connect failed %d\n", evt->result);
				break;
			}
			mqtt_connected = true;
			LOG_INF("MQTT client connected\n");
			break;
		case MQTT_EVT_DISCONNECT:
			LOG_INF("MQTT client disconnected %d\n", evt->result);
			mqtt_connected = false;
			break;
		case MQTT_EVT_PUBLISH:
			if (evt->result != 0){
				LOG_ERR("MQTT PUBLISH error %d\n", evt->result);
				break;
			}
			LOG_INF("MQTT PUBLISH successed!\n");
			break;

		default:
			break;
	}
}

static void broker_init(void){
	struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
	broker4->sin_family = AF_INET;
	broker4->sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);
}

static void client_init(struct mqtt_client *client){
	mqtt_client_init(client);
	broker_init();
	client->broker = &broker;
	client->evt_cb = mqtt_evt_handler;
	client->user_name = &username;
	client->password = &password;
	client->client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
	client->client_id.size = strlen(MQTT_CLIENTID);
	client->protocol_version = MQTT_VERSION_3_1_1;
	client->rx_buf = rx_buffer;
	client->rx_buf_size = sizeof(rx_buffer);
	client->tx_buf = tx_buffer;
	client->tx_buf_size = sizeof(tx_buffer);
	client->keepalive = DEFAULT_KEEPALIVE_SEC;
	client->transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static int try_to_connect(struct mqtt_client *client){
	int rc,i=0;
	while(i++<APP_CONNECT_TRIES && !mqtt_connected){
		client_init(client);
		rc = mqtt_connect(client);
		if(rc != 0){
			LOG_ERR("mqtt_connect failed %d\n", rc);
			k_sleep(K_MSEC(APP_SLEEP_MSECS));
			continue;
		}
		// 等待mqtt_evt_handler 进行MQTT_EVT_CONNACK处理 确认连接成功
		int timeout_ms = APP_CONNECT_TIMEOUT_MS;
		while(!mqtt_connected && timeout_ms > 0){
			k_sleep(K_MSEC(APP_SLEEP_MSECS));
			timeout_ms -= APP_SLEEP_MSECS;
			mqtt_input(client);
		}
		if (!mqtt_connected){
			mqtt_abort(client);
		}	
	}
	if(mqtt_connected){
		return 0;
	}
	return -EINVAL;
}

static int publish(struct mqtt_client *client, enum mqtt_qos qos){
	struct mqtt_publish_param param;
	param.message.topic.qos = qos;
	param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
	param.message.topic.topic.size = strlen(TOPIC);
	param.message.payload.data = (uint8_t *)PAYLOAD;
	param.message.payload.len = strlen(PAYLOAD);
	param.message_id = sys_rand16_get();
	param.dup_flag = 0U;
	param.retain_flag = 0U;
	return mqtt_publish(client, &param);
}

static void publisher(void){
	int i,rc,r = 0;
	LOG_INF("attempting to connect to MQTT broker\n");
	rc = try_to_connect(&client_ctx);
	if(rc != 0){
		LOG_ERR("try_to_connect failed %d\n", rc);
		return;
	}else{
		LOG_INF("MQTT broker connected\n");
	}
	while(1){
		mqtt_live(&client_ctx);
		rc = publish(&client_ctx, MQTT_QOS_0_AT_MOST_ONCE);
		if(rc != 0){
			LOG_ERR("mqtt_publish failed %d\n", rc);
		}else{
			LOG_INF("mqtt_publish successed\n");
		}
		k_sleep(K_SECONDS(5));
	}
}


int main(void){
	
	wifi_connect();
	publisher();
	return 0;
}