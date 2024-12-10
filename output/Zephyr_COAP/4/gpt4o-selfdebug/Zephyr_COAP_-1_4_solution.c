#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/coap_link_format.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(coap_client);

#define COAP_IP_ADDR "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_REQUEST_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in server_addr;
    struct zsock_addrinfo hints;
    struct zsock_addrinfo *res;
    struct coap_packet request;
    uint8_t request_buf[128];
    int sock;
    int r;

    LOG_INF("Starting CoAP client");

    // Prepare server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, COAP_IP_ADDR, &server_addr.sin_addr);

    // Prepare socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return;
    }

    // Initialize CoAP packet
    r = coap_packet_init(&request, request_buf, sizeof(request_buf),
                         1, COAP_TYPE_CON, 8, coap_next_token(),
                         COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        LOG_ERR("Failed to init CoAP packet");
        close(sock);
        return;
    }

    // Set CoAP URI path
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                  COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (r < 0) {
        LOG_ERR("Failed to append URI path");
        close(sock);
        return;
    }

    // Set CoAP payload
    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        LOG_ERR("Failed to append payload marker");
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)COAP_REQUEST_PAYLOAD,
                                   strlen(COAP_REQUEST_PAYLOAD));
    if (r < 0) {
        LOG_ERR("Failed to append payload");
        close(sock);
        return;
    }

    // Send CoAP request
    r = sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        LOG_ERR("Failed to send CoAP request");
        close(sock);
        return;
    }

    LOG_INF("CoAP request sent successfully");

    close(sock);
}