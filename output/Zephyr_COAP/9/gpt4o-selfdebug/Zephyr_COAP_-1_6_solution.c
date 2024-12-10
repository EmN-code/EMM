#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    struct coap_packet request;
    uint8_t request_data[128];
    struct sockaddr_in dest_addr;
    int ret;

    /* Initialize the destination server address */
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &dest_addr.sin_addr);

    /* Initialize the CoAP packet */
    ret = coap_packet_init(&request, request_data, sizeof(request_data),
                           COAP_VERSION_1, COAP_TYPE_CON,
                           COAP_TOKEN_LEN, coap_next_token(),
                           COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP request packet\n");
        return;
    }

    /* Set the URI path for the request */
    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                    COAP_URI_PATH, strlen(COAP_URI_PATH));
    if (ret < 0) {
        printk("Failed to append URI_PATH option\n");
        return;
    }

    /* Set the payload for the request */
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker\n");
        return;
    }

    ret = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD,
                                     strlen(COAP_PAYLOAD));
    if (ret < 0) {
        printk("Failed to append payload\n");
        return;
    }

    /* Send the CoAP request */
    ret = coap_send_request(&request, (struct sockaddr *)&dest_addr,
                            sizeof(dest_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        return;
    }

    printk("CoAP POST request sent successfully\n");
}