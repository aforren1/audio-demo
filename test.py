import struct
import hid
from pynput.mouse import Listener

h = hid.device()
h.open(0x16c0, 0x486)
h.set_nonblocking(1)

def on_move(x, y):
    print('x: %s, y: %s' % (x, y))
    byts = struct.pack('HH', x, y)
    h.write(byts)

with Listener(on_move=on_move) as listener:
    listener.join()
