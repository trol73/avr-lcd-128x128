#!/usr/bin/python
# -*- coding: windows-1251 -*-
import random
import time

import sys

from display import Display

__author__ = 'Trol'



#dev = Display('com4', 56000)
#dev = Display('/dev/tty.wchusbserial14230', 57600)
dev = Display('simulator://localhost', 3310)
#dev = Display('/dev/tty.wchusbserial14230', 9600)
#print dev.serial.baudrate

cnt = 0
for i in range(0, 100):
    r = dev.sync(i)
    if i == r:
        cnt += 1
        if cnt > 5:
            break
    else :
        cnt = 0
    print i, r

if cnt == 0:
    print "Can't connect to the device"
    sys.exit(1)

dev.beep(1000, 1)



dev.set_backlight_time(0)
#dev.fill_screen(Display.COLOR_RED)
#dev.fill_screen(Display.color(0.5, 1.0, 0.5))
dev.set_backlight_time(0xffff)
#time.sleep(0.5)
# dev.beep(1000, 50)
dev.fill_screen(Display.color(0.9, 0.8, 0.9))
dev.set_color(Display.COLOR_MAGENTA)
dev.set_background(Display.COLOR_BLUE)
dev.set_font(1)
dev.set_beeper_volume(255)
dev.set_backlight_level_high(255)

for n in range(0, 10000):
    tt = random.randint(1, 10)
    t = random.randint(2, 200/tt)
    dev.fill_string_xy(30, 50, ' ' + str(t) + ' ', 0)
    dev.fill_rect(25, 50, 2, 15)
    for i in range(t):
        dev.draw_progress_bar(10, 10, 108, 10, i*100/t)
        dev.draw_vertical_scrollbar(0, 0, 5, 127, i, t, 10)
        dev.sync(i)
        time.sleep(0.5)
    
1/0

#dev.fill_screen(Display.COLOR_BLUE)
#time.sleep(0.5)

#dev.fill_screen(Display.COLOR_WHITE)

dev.set_color(Display.COLOR_RED)
# dev.draw_line_by(0, 0, 127, 127, Display.COLOR_BLUE)
# dev.draw_line(0, 127, 127, 0)


# dev.draw_rect_thick_by(15, 15, 108, 108, 2, 2, Display.COLOR_GREEN)
# dev.draw_rect_thick(10, 10, 108, 108, 5, 5)
#
# dev.draw_rect(10, 10, 108, 108)
# dev.draw_rect_by(35, 35, 38, 38, Display.COLOR_GREEN)
# dev.draw_rect_by(55, 30, 3, 3, Display.COLOR_BLACK)

dev.sync(100)
dev.fill_screen(Display.COLOR_BLACK)
dev.sync(100)
dev.set_color(Display.COLOR_RED)
dev.fill_circle_by(50, 50, 45, Display.COLOR_CYAN)
dev.sync(100)
dev.fill_circle(100, 100, 20)

dev.fill_screen(Display.COLOR_BLACK)

dev.sync(100)
dev.set_font(1)

dev.set_color(Display.color(0, 1.0, 1))
dev.set_background(Display.COLOR_BLUE)
dev.fill_string_xy(0, 0, 'abcdefghijklmnopqrstuvwxyz.ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789,()[]<>', Display.ANCHOR_AUTO_WRAP)
1/0

# dev.sync(101)
# dev.sync(100)
# dev.sync(101)


dev.set_color(Display.color(1.0, 0.0, 1))
#dev.draw_string('абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ-0123456789,()[]<>')
#dev.sync(99)
dev.set_color(Display.color(0.9, 0.2, 1))
#dev.draw_string('abcdefghijklmnopqrstuvwxyz.ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789,()[]<>')
#dev.sync(100)
dev.set_color(Display.color(0.2, 1, 0))
#dev.draw_string('abcdefghijklmnopqrstuvwxyz.ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789,()[]<>')
#dev.sync(100)

print '102', dev.sync(102)
#dev.fill_screen(Display.COLOR_BLACK)
dev.set_color(Display.COLOR_YELLOW)
dev.set_background(Display.color(0.0, 0, 1.0))
dev.fill_string_xy(Display.WIDTH/2, 84, " -= str =- ", Display.ANCHOR_CENTER_H)
print '>', dev.sync(0)
#dev.fill_centered_string(95, " -= str =- ")
dev.fill_string_xy(0, 110, "ABCD", 0)
#print '>', dev.sync(0)
#print '>', dev.sync(1)

dev.set_backlight_time(0xffff)
dev.set_backlight_level_high(0xff)

dev.draw_string('abcdefghijklmnopqrstuvwxyz.ABCDEFGHIJKLMNOPQRSTUVWXYZ-0123456789,()[]<>')

dev.sync(5)
dev.set_background(Display.COLOR_BLUE)
dev.set_color(Display.COLOR_CYAN)
dev.fill_screen(Display.COLOR_BLACK)
dev.draw_string_xy(Display.WIDTH-30, 10, "TEST", Display.ANCHOR_RIGHT)

dev.draw_string_xy(Display.WIDTH, 30, "TEST2", Display.ANCHOR_RIGHT)
dev.fill_string_xy(Display.WIDTH, 45, "TEST2", Display.ANCHOR_RIGHT)
dev.set_color(Display.COLOR_GREEN)
dev.fill_string_xy(Display.WIDTH-2, 70, "TEST3", Display.ANCHOR_RIGHT)
dev.draw_string_xy(Display.WIDTH-2, 90, "Текст", Display.ANCHOR_RIGHT)


dev.fill_rect(10, 30, 10, 10)
dev.fill_rect_by(0, 10, Display.WIDTH, 10, Display.COLOR_RED)
dev.fill_string_xy(Display.WIDTH/2, Display.HEIGHT/2, "Текст", Display.ANCHOR_CENTER_V|Display.ANCHOR_CENTER_H)

#for i in range(0, 255):
#    dev.set_highlight_level(i)
#    dev.sync(1)


exit()


while True:
    #time.sleep(0.1)
    dev.sync(100)
    x = random.randint(0, Display.WIDTH-1)
    y = random.randint(0, Display.HEIGHT-1)
    w = random.randint(1, Display.WIDTH)
    h = random.randint(1, Display.HEIGHT)
    w = 20
    h = 20
    while w*h > Display.WIDTH * Display.HEIGHT/3:
        if x and 1 == 0:
            w /= 2
        else:
            h /= 2
    if x + w >= Display.WIDTH:
        x = Display.WIDTH - w
    if y + h >= Display.HEIGHT:
        y = Display.HEIGHT - h
    colors = [Display.COLOR_BLACK, Display.COLOR_BLUE, Display.COLOR_CYAN, Display.COLOR_GREEN, Display.COLOR_MAGENTA,
              Display.COLOR_RED, Display.COLOR_WHITE, Display.COLOR_YELLOW, Display.color(128, 0, 0),
              Display.color(0, 0.5, 0), Display.color(0, 0, 0.5), Display.color(0.5, 0.5, 0.5)]
    color = colors[random.randint(0, len(colors)-1)]
    dev.fill_rect_by(x, y, w, h, color)


'''
dev.draw_string_xy(10, 10, 'abcdefghijklmnopqrstuvwxyz')
dev.draw_char_xy(5, 5, '1')
dev.invert_pixel(40, 40)
dev.draw_line(0, 0, 10, 10, True)



dev.draw_centered_str(15, '1234567890ABCDE')
dev.lcd_write()

for i in range(1, 10):
    dev.lcd_clear()
    dev.draw_char_xy(5, 5, chr(ord('0') + i))
    #dev.beep(2000, 3)
    #dev.set_highlight(0xffff)
    dev.lcd_write()
    #dev.set_highlight(0xffff)
    time.sleep(0.5)


dev.set_highlight(1)
dev.set_keyboard_beep(False)
print dev.read_keyboard()
time.sleep(10)

dev._write(5)   # exit control mode
#while True:
#    print dev.read_keyboard()

'''