import usb.core
import usb.util
import struct
from pynput.mouse import Listener

device = usb.core.find(idVendor=0x16c0, idProduct=0x486)
for cfg in device:
  for intf in cfg:
    if device.is_kernel_driver_active(intf.bInterfaceNumber):
      try:
        device.detach_kernel_driver(intf.bInterfaceNumber)
      except usb.core.USBError as e:
        sys.exit("Could not detatch kernel driver from interface({0}): {1}".format(intf.bInterfaceNumber, str(e)))

interrupt_out = device[0][(0,0)][1]
#serial_in = device[0][(1,0)][0]

def on_move(x, y):
    byts = struct.pack('HH', x, y)
    interrupt_out.write(bytearray(byts))
    #data = serial_in.read(serial_in.wMaxPacketSize)
    print(x,y)
    #print(struct.unpack('c' * 10, data[:10]))

with Listener(on_move=on_move) as listener:
    listener.join()
