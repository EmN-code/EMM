
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/logging/log.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

LOG_MODULE_REGISTER(coap_client);

int main(void)
{
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket");
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP packet
    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        LOG_ERR("Failed to initialize CoAP packet");
        close(sock);
        return -1;
    }

    // Set the URI path
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        LOG_ERR("Failed to append URI path option");
        close(sock);
        return -1;
    }

    // Set the payload
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        LOG_ERR("Failed to append payload marker");
        close(sock);
        return -1;
    }

    ret = coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        LOG_ERR("Failed to append payload");
        close(sock);
        return -1;
    }

    // Send the CoAP request
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send CoAP request");
        close(sock);
        return -1;
    }

    LOG_INF("CoAP request sent successfully");

    close(sock);
    return 0;
}
