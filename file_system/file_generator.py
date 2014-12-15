from random import randrange
import numpy

array = [2, 4, 8, 16, 32, 64, 128]

for x in array:

  fp = open("size%sk" % x, "w+")

  content = numpy.random.bytes(x*1024*1024)

  fp.write(content);
  fp.close()