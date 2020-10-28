#!/usr/bin/python
# -*- coding: windows-1251 -*-
from display import Displayddddd

__author__ = 'Trol'

src = 0x8888FF

r = (src >> 16) & 0xff
g = (src >> 8) & 0xff
b = (src >> 0) & 0xff
clr = Display.color(r, g, b)
res = hex(clr)[2:].upper()
while len(res) < 4:
    res = '0' + res
res = '0x' + res
print res

