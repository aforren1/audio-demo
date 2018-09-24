import struct
import hid
from psychopy import core, visual, event, monitors

mon = monitors.Monitor('lappy', width=34)
mon.setSizePix([1920, 1080])

win = visual.Window(size=[800, 800], monitor=mon, screen=0, units='cm', fullscr=True)

ms = event.Mouse(win=win)

h = hid.device()
h.open(0x16c0, 0x486)
h.set_nonblocking(1)

while not any(event.getKeys()):
    x, y = ms.getPos()
    x /= 100
    y /= 100
    byts = struct.pack('ff', x, y)
    h.write(byts)
    print((x, y))
    win.flip()

h.close()
