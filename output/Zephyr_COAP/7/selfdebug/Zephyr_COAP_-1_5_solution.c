
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socketutils.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(coap_client);

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

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
        LOG_ERR("Failed to create socket (%d)", sock);
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Initialize the CoAP request
    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, 0, NULL, 0, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        LOG_ERR("Failed to initialize CoAP packet (%d)", ret);
        close(sock);
        return -1;
    }

    // Add payload to the CoAP request
    const char *payload = "Hello, COAP Cloud";
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        LOG_ERR("Failed to append payload marker (%d)", ret);
        close(sock);
        return -1;
    }

    ret = coap_packet_append_payload(&request, payload, strlen(payload));
    if (ret < 0) {
        LOG_ERR("Failed to append payload (%d)", ret);
        close(sock);
        return -1;
    }

    // Send the CoAP request
    ret = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        LOG_ERR("Failed to send CoAP request (%d)", ret);
        close(sock);
        return -1;
    }

    LOG_INF("CoAP request sent successfully");

    // Close the socket
    close(sock);

    return 0;
}
