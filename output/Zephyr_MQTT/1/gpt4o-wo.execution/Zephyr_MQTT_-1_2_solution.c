#include zephyr/types.h
#include zephyr/sys/util.h
#include zephyr/sys/slist.h
#include zephyr/sys/mem_stats.h
#include zephyr/sys/fdtable.h
#include zephyr/storage/flash_map.h
#include zephyr/syscalls/kernel.h
#include zephyr/syscalls/i2c.h
#include zephyr/syscalls/device.h
#include zephyr/syscalls.h
#include zephyr/sys/device_mmio.h
#include zephyr/net/wifi.h
#include zephyr/net/websocket.h
#include zephyr/net/tls_credentials.h
#include zephyr/net/tls_credentials_internal.h
#include zephyr/net/tls_socket.h
#include zephyr/net/tls.h
#include zephyr/net/tcp2.h
#include zephyr/net/tcp.h
#include zephyr/net/socket_select.h
#include zephyr/net/socket_poll.h
#include zephyr/net/socket_offload.h
#include zephyr/net/socket_net_mgmt.h
#include zephyr/net/socket_internal.h
#include zephyr/net/socket_can.h
#include zephyr/net/socket_af_unix.h
#include zephyr/net/socket.h
#include zephyr/net/sntp.h
#include zephyr/net/sntp_resolve.h
#include zephyr/net/sntp_fields.h
#include zephyr/net/socket/aio.h
#include zephyr/net/nppp.h
#include zephyr/net/nppp_internal.h
#include zephyr/net/net_transport_cmn.h
#include zephyr/net/net_transport.h
#include zephyr/net/net_timeout.h
#include zephyr/net/net_stats.h
#include zephyr/net/net_socket_logger.h
#include zephyr/net/net_socket.h
#include zephyr/net/net_pkt.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/net_l2_ethernet_internal.h
#include zephyr/net/net_l2_ethernet.h
#include zephyr/net/net_l2_internal.h
#include zephyr/net/net_ike.h
#include zephyr/net/net_if_stats.h
#include zephyr/net/net_if_offload.h
#include zephyr/net/net_if_internal.h
#include zephyr/net/net_if.h
#include zephyr/net/net_ethfp.h
#include zephyr/net/net_ethernet.h
#include zephyr/net/net_context_external_iface.h
#include zephyr/net/net_context_internal.h
#include zephyr/net/net_context.h
#include zephyr/net/net_coap.h
#include zephyr/net/net_can.h
#include zephyr/net/net_buf_pool.h
#include zephyr/net/net_buf.h
#include zephyr/net/net_arp.h
#include zephyr/net/mqtt_sn.h
#include zephyr/net/mqtt_sn_tokid.h
#include zephyr/net/mqtt_sn_flags.h
#include zephyr/net/mqtt_sn_common.h
#include zephyr/net/mqtt_sn_connack.h
#include zephyr/net/mqtt_sn_topic.h
#include zephyr/net/mqtt_sn_proto.h
#include zephyr/net/mqtt_sn_parser.h
#include zephyr/net/mqtt_sn.fields.h
#include zephyr/net/mqtt_sn_client.h
#include zephyr/net/mqtt_sn_cmd.h
#include zephyr/net/mqtt_common.h
#include zephyr/net/mqtt.h
#include zephyr/net/morso_internal.h
#include zephyr/net/modem/socket_manip.h
#include zephyr/net/modem/packets.h
#include zephyr/net/manip.h
#include zephyr/net/log.h
#include zephyr/net/http_client_cb.h
#include zephyr/net/http_parser.h
#include zephyr/net/http.h
#include zephyr/net/httx.h
#include zephyr/net/gptp_vg.h
#include zephyr/net/gptp_internal.h
#include zephyr/net/gptp.h
#include zephyr/net/gptp_fields.h
#include zephyr/net/gptp_fields_internal.h
#include zephyr/net/ftp.h
#include zephyr/net/ftp_manifest.h
#include zephyr/net/ftp_file_info.h
#include zephyr/net/ftp_download.h
#include zephyr/net/ftp_client.h
#include zephyr/net/ftp_cmd.h
#include zephyr/net/fsm_ftp.h
#include zephyr/net/coap_transport_internal.h
#include zephyr/net/coap_transport.h
#include zephyr/net/coap_packet.h
#include zephyr/net/coap_internal_apis.h
#include zephyr/net/coap.h
#include zephyr/net/buf_lock.h
#include zephyr/net/buf.h
#include zephyr/init.h
#include zephyr/fff/fff_common.h
#include zephyr/fff/fff.h
#include zephyr/devicetree/kc705_system_design.h
#include zephyr/devicetree/hifive_unleashed-funcs.h
#include zephyr/devicetree/ghrd-socfpga-funcs.h
#include zephyr/devicetree/fixed_partitions.h
#include zephyr/devicetree.h
#include zephyr/drivers/mmc/mmc_common.h
#include zephyr/drivers/mmc/mmc_common_defs.h
#include zephyr/drivers/led/led_stp16cp05.h
#include zephyr/drivers/led/led_stp16cp05_defs.h
#include zephyr/drivers/led.h
#include zephyr/drivers/flash/stm32_flash.h
#include zephyr/drivers/flash/host_common.h
#include zephyr/drivers/flash/esp32_flash.h
#include zephyr/drivers/flash.h
#include zephyr/canbus/port_common_defs.h
#include zephyr/canbus/port_common.h
#include zephyr/canbus/can_common.h
#include zephyr/canbus/can.h
#include zephyr/canbus/bus.h

#include <logging/log.h>
#include <zephyr.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "mqtt_example_client"
#define MQTT_TOPIC "state"

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct pollfd fds[1];

LOG_MODULE_REGISTER(mqtt_example, LOG_LEVEL_INF);

static void prepare_broker(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);
}

static int publish_message(struct mqtt_client *client, const char *topic, const char *message) {
    struct mqtt_publish_param param;
    
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = (uint8_t *)message;
    param.message.payload.len = strlen(message);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    return mqtt_publish(client, &param);
}

static int setup_mqtt_client(void) {
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;

    struct mqtt_sec_config *tls_config = &client.transport.tls.config;
    tls_config->peer_verify = 0;
    tls_config->cipher_count = 0;
    
    return mqtt_connect(&client);
}

void main(void) {
    int ret;
    
    ret = setup_mqtt_client();
    if (ret) {
        LOG_ERR("Failed to connect to the broker (%d)", ret);
        return;
    }
    
    prepare_broker();

    while (1) {
        mqtt_input(&client);
        mqtt_ping(&client);

        ret = publish_message(&client, MQTT_TOPIC, "work");
        if (ret) {
            LOG_ERR("Failed to publish message (%d)", ret);
        }

        k_sleep(K_SECONDS(5));
    }
}