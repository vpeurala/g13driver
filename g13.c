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

#define IRQ_CHANNEL 13

static struct usb_device_id skel_table[] = {
    { USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID) },
    { }
};

static struct file_operations g13_fops = {
    .owner = THIS_MODULE,
};

static struct usb_class_driver skel_class = {
    .name = "g13",
    .fops = &g13_fops,
    .minor_base = 13,
};

MODULE_DEVICE_TABLE(usb, skel_table);

static irqreturn_t g13_irq_handler(int irq, void *dev_id) {
    printk("Interrupt!\n");
    return IRQ_HANDLED;
};

static void in_complete(struct urb *urb) {
    printk("in_complete!\n");
    printk("status: %d\n", urb->status);
};

static int skel_probe(struct usb_interface *intf, const struct usb_device_id *id) {
    struct usb_device *device = interface_to_usbdev(intf);
    struct usb_host_interface *cur_altsetting = intf->cur_altsetting;
    struct usb_interface_descriptor desc = cur_altsetting->desc;
    int usb_register_dev_result; 
    int request_irq_result;
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
    printk("probe, %lu\n", id->driver_info);
    printk("cur_altsetting: %d\n", desc.iInterface);
    for (i = 0; i < desc.bNumEndpoints; i++) {
        endpoint = cur_altsetting->endpoint[i];
        endpoint_descriptor = endpoint.desc;
        bEndpointAddress = endpoint_descriptor.bEndpointAddress;
        bmAttributes = endpoint_descriptor.bmAttributes;
        if (usb_endpoint_dir_in(&endpoint_descriptor)) {
            printk("IN endpoint %d, attributes %d\n", bEndpointAddress, bmAttributes);
            /* We know that bmAttributes == USB_ENDPOINT_XFER_INT */
            if (usb_endpoint_xfer_int(&endpoint_descriptor)) {
                printk("XFER_INT\n");
                bInterval = endpoint_descriptor.bInterval;
                wMaxPacketSize = endpoint_descriptor.wMaxPacketSize;
                in_pipe = usb_rcvintpipe(device, bEndpointAddress);
                in_transfer_buffer = kmalloc(wMaxPacketSize, GFP_KERNEL);
                in_transfer_buffer_length = wMaxPacketSize; 
                urb = usb_alloc_urb(0, GFP_KERNEL);
                usb_fill_int_urb(urb, device, in_pipe, in_transfer_buffer, in_transfer_buffer_length, &in_complete, NULL, bInterval);
                usb_submit_urb(urb, GFP_KERNEL);
            }
        } else if (usb_endpoint_dir_out(&endpoint_descriptor)) {
            printk("OUT endpoint %d, attributes %d\n", bEndpointAddress, bmAttributes);
            /* We know that bmAttributes == USB_ENDPOINT_XFER_INT */
            if (usb_endpoint_xfer_int(&endpoint_descriptor)) {
                printk("XFER_INT\n");    
                bInterval = endpoint_descriptor.bInterval;
            }
        } else {
            printk("BUG!!! Endpoint not IN nor OUT.\n");
        } 
    }
    usb_register_dev_result = usb_register_dev(intf, &skel_class);
    if (usb_register_dev_result ) {
        printk("Not able to get a minor for this device\n");
        usb_set_intfdata(intf, NULL);
        return -1;
    }
    printk("usb_register_dev successful.\n");
    request_irq_result = request_irq(IRQ_CHANNEL, &g13_irq_handler, IRQF_SAMPLE_RANDOM, "g13", NULL); 
    printk("request_irq_result: %d\n", request_irq_result);
    return 0;
}

static void skel_disconnect(struct usb_interface *intf) {
    int minor = intf->minor;
    printk("disconnect start\n");
    usb_deregister_dev(intf, &skel_class);
    free_irq(IRQ_CHANNEL, NULL);
    printk("disconnect successful.\n");
}

static struct usb_driver skel_driver = {
    .name = "g13",
    .id_table = skel_table,
    .probe = skel_probe,
    .disconnect = skel_disconnect,
};

static int __init usb_skel_init(void) {
    int result;
    result = usb_register(&skel_driver);
    if (result) {
        printk("usb_register failed. Error number %d\n", result);
    } else {
        printk("usb_register successful.\n");
    }
    return result;
}

static void __exit usb_skel_exit(void) {
    usb_deregister(&skel_driver);
}

module_init(usb_skel_init);
module_exit(usb_skel_exit);
