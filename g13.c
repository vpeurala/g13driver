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

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Ville Peurala <ville.peurala@gmail.com>");
MODULE_DESCRIPTION("Driver for Logitech G13.");
MODULE_VERSION("0.1");

#define USB_VENDOR_ID 0x046D
#define USB_PRODUCT_ID 0xC21C

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
    char* transfer_buffer_content;
    u32 actual_length;
    transfer_buffer_content = (char*) urb->transfer_buffer;
    actual_length = urb->actual_length;
    /* FIXME VP 27.12.2010: Hardcoded A for every key */
    input_report_key(g13_input_device, KEY_A, 1);
    input_report_key(g13_input_device, KEY_A, 0);
    input_sync(g13_input_device);
    usb_submit_urb(urb, GFP_ATOMIC);
};

static int g13_probe(struct usb_interface *intf, const struct usb_device_id *id) {
    struct usb_device *device = interface_to_usbdev(intf);
    struct usb_host_interface *cur_altsetting = intf->cur_altsetting;
    struct usb_interface_descriptor desc = cur_altsetting->desc;
    int usb_register_dev_result; 
    int i;
    struct usb_host_endpoint endpoint;
    struct usb_endpoint_descriptor endpoint_descriptor;
    __u8 bEndpointAddress;
    __u8 bmAttributes;
    __u8 bInterval;
    struct urb *urb;
    unsigned int in_pipe;
    __le16 wMaxPacketSize;
    void *in_transfer_buffer;
    int in_transfer_buffer_length;
    int input_register_device_result;
    g13_input_device = input_allocate_device();
    if (g13_input_device == NULL) {
        printk("G13: input_allocate_device failed.\n");
        return -1;
    }
    g13_input_device->name = "G13";
    g13_input_device->evbit[0] = BIT(EV_KEY);
    set_bit(KEY_A, g13_input_device->keybit);
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
                in_transfer_buffer = kmalloc(wMaxPacketSize, GFP_ATOMIC);
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

