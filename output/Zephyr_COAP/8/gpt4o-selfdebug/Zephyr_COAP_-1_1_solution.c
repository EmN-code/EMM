#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define POST_DATA "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

int main(void) {
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t request_buffer[128];
    int r;
    
    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }
    
    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    
    // Initialize CoAP request
    coap_packet_init(&request, request_buffer, sizeof(request_buffer), COAP_VERSION_1, COAP_TYPE_CON, 
                     0, NULL, COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, (uint8_t *)URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)POST_DATA, strlen(POST_DATA));

    // Send CoAP request
    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP request sent successfully\n");

    // Close socket
    close(sock);
    
    return 0;
}