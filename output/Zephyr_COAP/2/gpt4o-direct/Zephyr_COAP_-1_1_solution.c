
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

LOG_MODULE_REGISTER(coap_client);

void send_coap_request(int sock, struct sockaddr_in *server_addr) {
    struct coap_packet request;
    struct coap_option options[1];
    uint8_t buffer[128];
    int ret;

    // Initialize CoAP packet
    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, 0);
    if (ret < 0) {
        LOG_ERR("Failed to initialize CoAP packet");
        return;
    }

    // Add URI path option
    ret = coap_append_option_int(&request, COAP_OPTION_URI_PATH, strlen(URI_PATH), URI_PATH);
    if (ret < 0) {
        LOG_ERR("Failed to append URI path option");
        return;
    }

    // Add payload
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        LOG_ERR("Failed to append payload marker");
        return;
    }

    ret = coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        LOG_ERR("Failed to append payload");
        return;
    }

    // Send CoAP request
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send CoAP request");
        return;
    }

    LOG_INF("CoAP request sent successfully");
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    // Initialize network stack
    if (net_init() < 0) {
        LOG_ERR("Failed to initialize network stack");
        return -1;
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return -1;
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Send CoAP request
    send_coap_request(sock, &server_addr);

    // Close socket
    close(sock);

    return 0;
}