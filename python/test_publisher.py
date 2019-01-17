import Publisher
import time
import struct

pub = Publisher.Publisher(8888)


x_img = 333.2
y_img = 222.3
a_img = 0.532

while True:
    bin_data = struct.pack("fff", x_img, y_img, a_img)
    pub.publish(bin_data)
    print("sending")
    time.sleep(1.0)