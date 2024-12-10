#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/logging/log.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

#define COAP_MESSAGE "Hello, COAP Cloud"

LOG_MODULE_REGISTER(coap_client, LOG_LEVEL_DBG);

int main(void) {
    struct sockaddr_in serv_addr;
    int sock, ret;
    struct coap_packet request;
    uint8_t request_buf[256];
    uint8_t payload[] = COAP_MESSAGE;
    const char *uri_path[] = {COAP_URI_PATH, NULL};

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        return -1;
    }

    // Setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &serv_addr.sin_addr);

    // Initialize CoAP packet
    ret = coap_packet_init(&request, request_buf, sizeof(request_buf),
                           1, COAP_TYPE_CON, 8, coap_next_token(),
                           COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        LOG_ERR("Failed to init CoAP packet: %d", ret);
        close(sock);
        return -1;
    }

    // Set the URI path
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                    uri_path, strlen(COAP_URI_PATH));
    if (ret < 0) {
        LOG_ERR("Failed to append URI path: %d", ret);
        close(sock);
        return -1;
    }

    // Set the payload
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        LOG_ERR("Failed to append payload marker: %d", ret);
        close(sock);
        return -1;
    }

    ret = coap_packet_append_payload(&request, payload, sizeof(payload) - 1);
    if (ret < 0) {
        LOG_ERR("Failed to append payload: %d", ret);
        close(sock);
        return -1;
    }

    // Send CoAP request
    ret = sendto(sock, request.data, request.offset, 0,
                 (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send CoAP request: %d", errno);
        close(sock);
        return -1;
    }

    LOG_INF("CoAP message sent to %s:%d", COAP_SERVER_IP, COAP_SERVER_PORT);

    close(sock);
    return 0;
}