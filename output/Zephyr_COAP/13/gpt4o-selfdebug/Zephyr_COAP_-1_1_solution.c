#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr.h>

#define COAP_IP_ADDRESS "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct coap_socket sock;

int main() {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    const char *payload = "Hello, COAP Cloud";
    int ret;

    k_sleep(K_SECONDS(5)); // Delay to ensure network readiness

    // Create CoAP socket
    ret = coap_socket_create(&sock, AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ret < 0) {
        printk("Failed to create CoAP socket\n");
        return -1;
    }

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, COAP_IP_ADDRESS, &server_addr.sin_addr);

    // Initialize CoAP request
    coap_packet_init(&request, COAP_TYPE_CON, COAP_METHOD_POST, 
                     coap_next_token(), COAP_TYPE_CON);

    // Set URI path
    coap_packet_append_uri_path(&request, COAP_URI_PATH);

    // Set payload
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    // Send request
    ret = coap_sendto(&sock, &request, &server_addr);
    if (ret < 0) {
        printk("CoAP request failed\n");
        return -1;
    }

    printk("CoAP POST request sent successfully\n");

    return 0;
}