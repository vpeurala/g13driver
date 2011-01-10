#include <linux/bitops.h>
#include <linux/bitmap.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/usb.h>
#include <linux/usb/ch9.h>

#include "g13_util.c"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Ville Peurala <ville.peurala@gmail.com>");
MODULE_DESCRIPTION("Driver for Logitech G13.");
MODULE_VERSION("0.1");

#define USB_VENDOR_ID 0x046D
#define USB_PRODUCT_ID 0xC21C

#define REGISTER_BUTTON(index) g13_input_device->keybit[BIT_WORD(BTN_TRIGGER_HAPPY ## index)] = BIT_MASK(BTN_TRIGGER_HAPPY ## index);

#ifndef REGISTER_BUTTON
#define REPORT_BUTTON(button_index, bit_index) \
    if (test_bit(bit_index, input_bitmap)) { \
        printk("g%i pressed\n", button_index); \
        input_report_key(g13_input_device, BTN_TRIGGER_HAPPY ## button_index, 1); \
    } else { \
        printk("g%i released\n", button_index); \
        input_report_key(g13_input_device, BTN_TRIGGER_HAPPY ## button_index, 0); \
    }
#endif

#define REPORT_BUTTON(button_index, bit_index) \
    printk("%i", test_bit(bit_index, input_bitmap));

static struct input_dev* g13_input_device; 

static struct usb_device_id g13_device_id[] = {
    { USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID) },
    { }
};

static struct file_operations g13_fops = {
    .owner = THIS_MODULE,
};

static struct usb_class_driver g13_class = {
    .name = "G13",
    .fops = &g13_fops,
    .minor_base = 13,
};

MODULE_DEVICE_TABLE(usb, g13_device_id);

static void g13_urb_complete(struct urb *urb) {
    const u8* transfer_buffer_content = (unsigned char*) urb->transfer_buffer;
    /* actual_length is always 8 with G13 input channel */
    const u32 actual_length = urb->actual_length;
    int bm_index = 0;
    int safety_limit_index = 0;
    int bitmap_parse_result;
    int sprintf_result;
    int i;
    char hex_string[16];
    DECLARE_BITMAP(input_bitmap, 64);
    printk("\n");
    for (i = 0; i < actual_length; i++) {
        printk("%x ", transfer_buffer_content[i]);
    }
    sprintf_result = sprintf(hex_string, "%02x%02x%02x%02x,%02x%02x%02x%02x", transfer_buffer_content[0], transfer_buffer_content[1], transfer_buffer_content[2], transfer_buffer_content[3], transfer_buffer_content[4], transfer_buffer_content[5], transfer_buffer_content[6], transfer_buffer_content[7]); 
    printk("sprintf_result: %d\n", sprintf_result);
    printk("hex_string: %s\n", hex_string);
    printk("\n");
    bitmap_parse_result = bitmap_parse(hex_string, 33, &input_bitmap, 64);
    printk("transfer: %s\n", eight_bytes_to_bit_string(transfer_buffer_content));
    printk("bitmap_parse_result: %d\n", bitmap_parse_result);
    REPORT_BUTTON(1, 1-1);
    REPORT_BUTTON(2, 2-1);
    REPORT_BUTTON(3, 3-1);
    REPORT_BUTTON(4, 4-1);
    REPORT_BUTTON(5, 5-1);
    REPORT_BUTTON(6, 6-1);
    REPORT_BUTTON(7, 7-1);
    REPORT_BUTTON(8, 8-1);
    REPORT_BUTTON(9, 9-1);
    REPORT_BUTTON(10, 10-1);
    REPORT_BUTTON(11, 11-1);
    REPORT_BUTTON(12, 12-1);
    REPORT_BUTTON(13, 13-1);
    REPORT_BUTTON(14, 14-1);
    REPORT_BUTTON(15, 15-1);
    REPORT_BUTTON(16, 16-1);
    REPORT_BUTTON(17, 17-1);
    REPORT_BUTTON(18, 18-1);
    REPORT_BUTTON(19, 19-1);
    REPORT_BUTTON(20, 20-1);
    REPORT_BUTTON(21, 21-1);
    REPORT_BUTTON(22, 22-1);
    REPORT_BUTTON(23, 23-1);
    REPORT_BUTTON(24, 24-1);
    REPORT_BUTTON(25, 25-1);
    REPORT_BUTTON(26, 26-1);
    REPORT_BUTTON(27, 27-1);
    REPORT_BUTTON(28, 28-1);
    REPORT_BUTTON(29, 29-1);
    REPORT_BUTTON(30, 30-1);
    REPORT_BUTTON(31, 31-1);
    REPORT_BUTTON(32, 32-1);
    REPORT_BUTTON(33, 33-1);
    REPORT_BUTTON(34, 34-1);
    REPORT_BUTTON(35, 35-1);
    REPORT_BUTTON(36, 36-1);
    REPORT_BUTTON(37, 37-1);
    REPORT_BUTTON(38, 38-1);
    REPORT_BUTTON(39, 39-1);
    REPORT_BUTTON(40, 40-1);
    REPORT_BUTTON(41, 41-1);
    REPORT_BUTTON(42, 42-1);
    REPORT_BUTTON(43, 43-1);
    REPORT_BUTTON(44, 44-1);
    REPORT_BUTTON(45, 45-1);
    REPORT_BUTTON(46, 46-1);
    REPORT_BUTTON(47, 47-1);
    REPORT_BUTTON(48, 48-1);
    REPORT_BUTTON(49, 49-1);
    REPORT_BUTTON(50, 50-1);
    REPORT_BUTTON(51, 51-1);
    REPORT_BUTTON(52, 52-1);
    REPORT_BUTTON(53, 53-1);
    REPORT_BUTTON(54, 54-1);
    REPORT_BUTTON(55, 55-1);
    REPORT_BUTTON(56, 56-1);
    REPORT_BUTTON(57, 57-1);
    REPORT_BUTTON(58, 58-1);
    REPORT_BUTTON(59, 59-1);
    REPORT_BUTTON(60, 60-1);
    REPORT_BUTTON(61, 61-1);
    REPORT_BUTTON(62, 62-1);
    REPORT_BUTTON(63, 63-1);
    input_sync(g13_input_device);
    /* FIXME VP 27.12.2010: Hardcoded A for every key */
    /*
    input_report_key(g13_input_device, KEY_A, 1);
    input_report_key(g13_input_device, KEY_A, 0);
    input_sync(g13_input_device);
    */
    /* TODO VP 30.12.2010: Do we actually need this memset? */
    memset(urb->transfer_buffer, '\0', urb->transfer_buffer_length);
    usb_submit_urb(urb, GFP_ATOMIC);
};

/* FIXME VP 27.12.2010: Really long method */
static int g13_probe(struct usb_interface *intf, const struct usb_device_id *id) {
    struct usb_device* device = interface_to_usbdev(intf);
    struct usb_host_interface* cur_altsetting = intf->cur_altsetting;
    struct usb_interface_descriptor desc = cur_altsetting->desc;
    int usb_register_dev_result; 
    int i;
    struct usb_host_endpoint endpoint;
    struct usb_endpoint_descriptor endpoint_descriptor;
    __u8 bEndpointAddress;
    __u8 bmAttributes;
    __u8 bInterval;
    struct urb* urb;
    unsigned int in_pipe;
    __le16 wMaxPacketSize;
    unsigned char* in_transfer_buffer;
    unsigned int in_transfer_buffer_length;
    int input_register_device_result;
    g13_input_device = input_allocate_device();
    if (g13_input_device == NULL) {
        printk("G13: input_allocate_device failed.\n");
        return -1;
    }
    g13_input_device->name = "G13";
    g13_input_device->evbit[0] = BIT_MASK(EV_KEY);
    REGISTER_BUTTON(1);
    REGISTER_BUTTON(2);
    REGISTER_BUTTON(3);
    REGISTER_BUTTON(4);
    REGISTER_BUTTON(5);
    REGISTER_BUTTON(6);
    REGISTER_BUTTON(7);
    REGISTER_BUTTON(8);
    REGISTER_BUTTON(9);
    REGISTER_BUTTON(10);
    REGISTER_BUTTON(11);
    REGISTER_BUTTON(12);
    REGISTER_BUTTON(13);
    REGISTER_BUTTON(14);
    REGISTER_BUTTON(15);
    REGISTER_BUTTON(16);
    REGISTER_BUTTON(17);
    REGISTER_BUTTON(18);
    REGISTER_BUTTON(19);
    REGISTER_BUTTON(20);
    REGISTER_BUTTON(21);
    REGISTER_BUTTON(22);
    input_register_device_result = input_register_device(g13_input_device);
    if (input_register_device_result) {
        printk("G13: input_register_device failed: %d\n", input_register_device_result);
        return input_register_device_result;
    }
    for (i = 0; i < desc.bNumEndpoints; i++) {
        endpoint = cur_altsetting->endpoint[i];
        endpoint_descriptor = endpoint.desc;
        bEndpointAddress = endpoint_descriptor.bEndpointAddress;
        bmAttributes = endpoint_descriptor.bmAttributes;
        if (usb_endpoint_dir_in(&endpoint_descriptor)) {
            /* We know that bmAttributes == USB_ENDPOINT_XFER_INT */
            if (usb_endpoint_xfer_int(&endpoint_descriptor)) {
                bInterval = endpoint_descriptor.bInterval;
                wMaxPacketSize = endpoint_descriptor.wMaxPacketSize;
                in_pipe = usb_rcvintpipe(device, bEndpointAddress);
                in_transfer_buffer = kzalloc((sizeof (unsigned char)) * wMaxPacketSize, GFP_ATOMIC);
                in_transfer_buffer_length = wMaxPacketSize; 
                urb = usb_alloc_urb(0, GFP_ATOMIC);
                usb_fill_int_urb(urb, device, in_pipe, in_transfer_buffer, in_transfer_buffer_length, &g13_urb_complete, NULL, bInterval);
                usb_submit_urb(urb, GFP_ATOMIC);
            }
        } else if (usb_endpoint_dir_out(&endpoint_descriptor)) {
            /* We know that bmAttributes == USB_ENDPOINT_XFER_INT */
            if (usb_endpoint_xfer_int(&endpoint_descriptor)) {
                bInterval = endpoint_descriptor.bInterval;
                /* TODO VP 27.12.2010: Implement output */
            }
        } else {
            printk("G13: Bug found! Endpoint not IN nor OUT.\n");
        } 
    }
    usb_register_dev_result = usb_register_dev(intf, &g13_class);
    if (usb_register_dev_result ) {
        printk("G13: usb_register_dev failed: %d\n", usb_register_dev_result);
        return usb_register_dev_result;
    }
    printk("G13: Device registration successful.\n");
    return 0;
}

static void g13_disconnect(struct usb_interface *intf) {
    usb_deregister_dev(intf, &g13_class);
    input_unregister_device(g13_input_device);
    input_free_device(g13_input_device);
}

static struct usb_driver g13_driver = {
    .name = "G13",
    .id_table = g13_device_id,
    .probe = g13_probe,
    .disconnect = g13_disconnect,
};

static int __init g13_init(void) {
    int result;
    result = usb_register(&g13_driver);
    if (result) {
        printk("G13: usb_register failed. Error number %d.\n", result);
    } else {
        printk("G13: usb_register successful.\n");
    }
    return result;
}

static void __exit g13_exit(void) {
    usb_deregister(&g13_driver);
}

module_init(g13_init);
module_exit(g13_exit);

