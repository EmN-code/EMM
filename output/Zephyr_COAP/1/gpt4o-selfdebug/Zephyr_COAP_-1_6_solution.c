#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>
#include <net/net_ip.h>
#include <net/net_core.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <logging/log.h>

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

#define COAP_MSG_TYPE COAP_TYPE_CON
#define COAP_METHOD COAP_METHOD_POST
#define COAP_CONTENT_FORMAT COAP_CONTENT_FORMAT_TEXT_PLAIN

#define STACK_SIZE 1024
#define PRIORITY 7

LOG_MODULE_REGISTER(coap_client_sample, LOG_LEVEL_DBG);

static struct sockaddr_in6 addr;

void coap_init(void) {
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET;
    addr.sin6_port = htons(COAP_SERVER_PORT);
    net_addr_pton(AF_INET, COAP_SERVER_ADDR, &addr.sin6_addr);
}

void coap_send_request(void) {
    struct coap_packet request;
    uint8_t request_data[128];
    int r;

    r = coap_packet_init(&request, request_data, sizeof(request_data),
                     COAP_VERSION_1, COAP_MSG_TYPE, 0, NULL, COAP_METHOD,
                     coap_next_id());
    if (r < 0) {
        LOG_ERR("Failed to initialize CoAP request");
        return;
    }

    r = coap_packet_append_uri_path_from_string(&request, COAP_URI_PATH);
    if (r < 0) {
        LOG_ERR("Failed to append URI path");
        return;
    }

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        LOG_ERR("Failed to append payload marker");
        return;
    }

    const char *payload = "Hello, COAP Cloud";
    r = coap_packet_append_payload(&request, (const uint8_t *)payload, strlen(payload));
    if (r < 0) {
        LOG_ERR("Failed to append payload");
        return;
    }

    int sock = socket(addr.sin6_family, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create UDP socket");
        return;
    }

    r = sendto(sock, request_data, coap_packet_get_len(&request), 0,
           (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        LOG_ERR("Failed to send CoAP request");
        close(sock);
        return;
    }

    LOG_INF("CoAP request sent to %s:%d", COAP_SERVER_ADDR, COAP_SERVER_PORT);
    close(sock);
}

void main(void) {
    coap_init();
    while (1) {
        coap_send_request();
        k_sleep(K_SECONDS(10));
    }
}