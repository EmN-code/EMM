#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(coap_client, LOG_LEVEL_DBG);

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "coap/test"
#define COAP_CONTENT "Hello, COAP Cloud"

int main(void) {
    struct sockaddr_in server_addr;
    struct zsock_addrinfo *addr_res;
    struct coap_packet request;
    uint8_t request_buf[256];
    uint8_t response_buf[256];
    int sock;
    int err;
    
    // Get server address info
    err = getaddrinfo(SERVER_ADDR, NULL, NULL, &addr_res);
    if (err != 0) {
        LOG_ERR("Failed to get address info: %d", err);
        return -1;
    }
    server_addr = *(struct sockaddr_in *)addr_res->ai_addr;
    server_addr.sin_port = htons(SERVER_PORT);
    
    // Create CoAP socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", errno);
        freeaddrinfo(addr_res);
        return -1;
    }
    
    // Initialize CoAP request
    coap_packet_init(&request, request_buf, sizeof(request_buf),
                     COAP_VERSION_1, COAP_TYPE_CON, COAP_TOKEN_MAX_LEN,
                     coap_next_id(), COAP_METHOD_POST, coap_next_token());
    
    // Set CoAP URI path
    const char *uri_path = COAP_PATH;
    coap_packet_append_uri_path_from_string(&request, uri_path);
    
    // Append payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_CONTENT, sizeof(COAP_CONTENT) - 1);
    
    // Send CoAP request
    err = sendto(sock, request.data, request.offset, 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (err < 0) {
        LOG_ERR("Failed to send CoAP request: %d", errno);
        close(sock);
        freeaddrinfo(addr_res);
        return -1;
    }
    
    // Receive CoAP response
    struct sockaddr from_addr;
    socklen_t from_addr_len = sizeof(from_addr);
    err = recvfrom(sock, response_buf, sizeof(response_buf), 0,
                   &from_addr, &from_addr_len);
    if (err < 0) {
        LOG_ERR("Failed to receive CoAP response: %d", errno);
    } else {
        LOG_INF("Received response of length %d", err);
    }
    
    // Clean up
    close(sock);
    freeaddrinfo(addr_res);
    return 0;
}