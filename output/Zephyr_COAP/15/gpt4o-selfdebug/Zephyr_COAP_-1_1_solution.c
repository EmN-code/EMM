#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define MAX_COAP_MSG_LEN 256

int main(void) {
    struct coap_packet request;
    uint8_t payload[] = "Hello, COAP Cloud";
    uint8_t coap_buffer[MAX_COAP_MSG_LEN];
    int sock;
    struct sockaddr_in server_addr;
    int r;

    /* Initialize server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    /* Create socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    /* Initialize CoAP packet */
    r = coap_packet_init(&request, coap_buffer, sizeof(coap_buffer),
                         1, COAP_TYPE_CON, 8, coap_next_id(),
                         COAP_METHOD_POST, COAP_TOKEN_NONE);
    if (r < 0) {
        printk("Failed to init CoAP packet\n");
        close(sock);
        return -1;
    }

    /* Set URI path */
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, 
                                  COAP_PATH, strlen(COAP_PATH));
    if (r < 0) {
        printk("Failed to set CoAP option URI_PATH\n");
        close(sock);
        return -1;
    }

    /* Set payload */
    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to set payload marker\n");
        close(sock);
        return -1;
    }

    r = coap_packet_append_payload(&request, payload, sizeof(payload) - 1);
    if (r < 0) {
        printk("Failed to append payload\n");
        close(sock);
        return -1;
    }

    /* Send CoAP request */
    r = sendto(sock, request.data, request.offset, 0, 
               (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP POST request sent\n");

    /* Close the socket */
    close(sock);

    return 0;
}
