#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

/* Thread stack size and priority */
#define STACK_SIZE 1024
#define PRIORITY 7

void main(void) {
    /* Server details */
    const char *server_addr = "47.102.103.1";
    const char *uri_path = "/coap/test";
    const uint16_t server_port = 5683;
    
    /* CoAP message buffer */
    uint8_t coap_buf[256];
    
    /* CoAP endpoint socket address */
    struct sockaddr_in server_sock;
    
    /* Socket descriptor */
    int sock;
    
    /* Initialize the server socket details */
    server_sock.sin_family = AF_INET;
    server_sock.sin_port = htons(server_port);
    zsock_inet_pton(AF_INET, server_addr, &server_sock.sin_addr);
    
    /* Create UDP socket */
    sock = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }
    
    while (1) {
        /* Build CoAP packet */
        struct coap_packet request;
        coap_packet_init(&request, coap_buf, sizeof(coap_buf), 1, COAP_TYPE_CON, 8, coap_next_id(), COAP_METHOD_POST, NULL);
        coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));
        coap_packet_append_payload_marker(&request);
        coap_packet_append_payload(&request, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
        
        /* Send CoAP packet */
        if (zsock_sendto(sock, coap_buf, request.hdr.len, 0, (struct sockaddr *)&server_sock, sizeof(server_sock)) < 0) {
            printk("Failed to send CoAP packet\n");
        }
        
        /* Wait for 10 seconds */
        k_sleep(K_SECONDS(10));
    }

    /* Close the socket */
    zsock_close(sock);
}