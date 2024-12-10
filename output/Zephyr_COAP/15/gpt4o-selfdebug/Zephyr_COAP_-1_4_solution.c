// Include header files necessary for using networking and the CoAP protocol in Zephyr
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define POST_PAYLOAD "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

void send_coap_post_request(void) {
    struct sockaddr_in server_addr = {0};
    int sock;
    struct coap_packet request;
    uint8_t request_data[256];
    uint8_t token[8] = {0};    
    ssize_t len;

    // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // Create socket for sending the CoAP request
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    // Initialize the CoAP packet
    coap_packet_init(&request, request_data, sizeof(request_data), 1, COAP_TYPE_CON, 8, token,
                     COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)POST_PAYLOAD, sizeof(POST_PAYLOAD) - 1);

    // Send CoAP POST request
    len = sendto(sock, request_data, request.offset, 0, (struct sockaddr *)&server_addr, 
                 sizeof(server_addr));
    if (len < 0) {
        printk("Failed to send request\n");
    }

    close(sock);
}

int main(void) {
    printk("CoAP client example\n");
    while (1) {
        send_coap_post_request();
        k_sleep(K_SECONDS(10));  // Send every 10 seconds
    }
}