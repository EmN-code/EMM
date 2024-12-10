#include <zephyr/kernel.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/zephyr.h>

#define STACK_SIZE 1024
#define PRIORITY 7

void coap_send_request(void) {
    struct sockaddr_in server_address;
    int sock;
    struct coap_packet request;
    uint8_t request_buffer[256];
    const char *coap_message = "Hello, COAP Cloud";
    struct coap_option options[2];

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5683);
    inet_pton(AF_INET, "47.102.103.1", &server_address.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create UDP socket\n");
        return;
    }

    coap_packet_init(&request, request_buffer, sizeof(request_buffer), COAP_VERSION_1, COAP_TYPE_CON, 0, NULL, COAP_METHOD_POST, coap_next_token());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, "/coap/test", strlen("/coap/test"));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, coap_message, strlen(coap_message));

    if (sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printk("Failed to send CoAP request\n");
    }

    close(sock);
}

static K_THREAD_STACK_DEFINE(thread_stack, STACK_SIZE);
static struct k_thread thread_data;

void main(void) {
    printk("Starting CoAP client\n");
    k_thread_create(&thread_data, thread_stack, STACK_SIZE,
                    (k_thread_entry_t)coap_send_request, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
}
