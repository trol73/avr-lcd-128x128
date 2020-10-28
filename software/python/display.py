# -*- coding: utf-8 -*-
__author__ = 'Trol'

# Установка:
# python -m pip install pyserial

# Формат цвета: rrrrrggg:gggbbbbb

import serial
import socket



def _bytes(i):
    return divmod(i, 0x100)


class Display:
   CMD_GET_VERSION = 0
   CMD_SYNC = 1
   CMD_START_BOOTLOADER = 2
   CMD_SET_BACKLIGHT_TIME = 3
   CMD_SET_BACKLIGHT_LEVEL_LOW = 4
   CMD_SET_BACKLIGHT_LEVEL_HIGH = 5
   CMD_SET_BEEPER_VOLUME = 6
   CMD_SET_KEYBOARD_BEEP = 7
   CMD_READ_KEYBOARD = 8
   CMD_BEEP = 9
   CMD_PLAY_MELODY = 10
   CMD_SET_STRING_INTERVAL_X = 11
   CMD_SET_STRING_INTERVAL_Y = 12
   CMD_SET_COLOR = 13
   CMD_SET_BACKGROUND = 14
   CMD_SET_FONT = 15
   CMD_DRAW_PIXEL = 16
   CMD_DRAW_LINE_DEFAULT = 17
   CMD_FILL_SCREEN = 18
   CMD_DRAW_LINE = 19
   CMD_DRAW_LINE_TO = 20
   CMD_DRAW_RECT_DEFAULT = 21
   CMD_DRAW_RECT = 22
   CMD_DRAW_RECT_THICK_DEFAULT = 23
   CMD_DRAW_RECT_THICK = 24
   CMD_FILL_RECT = 25
   CMD_FILL_RECT_DEFAULT = 26
   CMD_DRAW_CIRCLE_DEFAULT = 27
   CMD_DRAW_CIRCLE = 28
   CMD_FILL_CIRCLE_DEFAULT = 29
   CMD_FILL_CIRCLE = 30
   CMD_DRAW_CHAR = 31
   CMD_FILL_CHAR = 32
   CMD_DRAW_CHAR_XY = 33
   CMD_FILL_CHAR_XY = 34
   CMD_DRAW_STRING = 35
   CMD_FILL_STRING = 36
   CMD_DRAW_STRING_XY = 37
   CMD_SET_FILL_STRING_REGION = 38
   CMD_FILL_STRING_XY = 39


   int CMD_DRAW_VERTICAL_SCROLLBAR = 40
   int CMD_DRAW_PROGRESSBAR = 41


    KEY_UP = 0
    KEY_DOWN = 1
    KEY_LEFT = 2
    KEY_RIGHT = 3
    KEY_ENTER = 4


    WIDTH = 128
    HEIGHT = 128


    ANCHOR_LEFT = 0
    ANCHOR_RIGHT = 1
    ANCHOR_CENTER_H = 2
    ANCHOR_TOP = 0
    ANCHOR_BOTTOM = 4
    ANCHOR_CENTER_V = 8
    ANCHOR_FULL_LINE = 16
    ANCHOR_AUTO_WRAP = 32
    ANCHOR_CENTER = ANCHOR_CENTER_V | ANCHOR_CENTER_H

    COLOR_BLACK = 0x0000
    COLOR_BLUE = 0x001F
    COLOR_RED = 0xF800
    COLOR_GREEN = 0x07E0
    COLOR_CYAN = 0x07FF
    COLOR_MAGENTA = 0xF81F
    COLOR_YELLOW = 0xFFE0
    COLOR_WHITE = 0xFFFF
    # Brown
    # Gray
    # Dark Gray
    # Dark Blue
    # Dark Green
    # Dark Cyan
    # Dark Red
    # Dark Magenta

    def __init__(self, port_name, bauds):
        if port_name.startswith('simulator://'):
            host = port_name[len('simulator://'):]
            self.sock = socket.socket()
            self.sock.connect((host, bauds))
            self.serial = None
            print 'Simulator connected'
        else:
            self.serial = serial.serial_for_url(port_name, baudrate=bauds, timeout=1.0)
            self.sock = None
        self.prefix = -1
        res1 = self.sync(1)
        if res1 != 1 and res1 > 0:
            cnt = 1
            while True:
                try:
                    self._read()
                    cnt += 1
                except:
                    break
            print 'skip', cnt, 'bytes'
        self.sync(100)

    def init(self):
        self._read_all()

    def close(self):
        self.serial.close()


    def get_version(self):
        pass # TODO

    def sync(self, val):
        self._cmd(Display.CMD_SYNC, val)
        try:
            return self._read()
        except:
            return -1

    def set_backlight_time(self, val16):
        self._cmd(Display.CMD_SET_BACKLIGHT_TIME, _bytes(val16))

    def set_backlight_level_high(self, level):
        self._cmd(Display.CMD_SET_BACKLIGHT_LEVEL_HIGH, level)

    def set_backlight_level_low(self, level):
        self._cmd(Display.CMD_SET_BACKLIGHT_LEVEL_LOW, level)

    def set_keyboard_beep(self, enable):
        self._cmd(Display.CMD_SET_KEYBOARD_BEEP, 1 if enable else 0)

    def set_beeper_volume(self, volume):
        self._cmd(Display.CMD_SET_BEEPER_VOLUME, volume)
        
    def read_keyboard(self):
        self._cmd(Display.CMD_READ_KEYBOARD)
        mask = self._read()
        if mask == 0:
            return None
        result = []
        for i in range(0, 5):
            if (mask & (1 << i)) != 0:
                result.append(self._read())
            else:
                result.append(0)
        return result

    def beep(self, freq, duration):
        self._cmd(Display.CMD_BEEP, _bytes(freq), duration)

    def play_melody(self):
        pass # TODO

    def set_string_interval_x(self, interval):
        self._cmd(Display.CMD_SET_STRING_INTERVAL_X, interval)

    def set_string_interval_y(self, interval):
        self._cmd(Display.CMD_SET_STRING_INTERVAL_Y, interval)

    def set_color(self, color):
        self._cmd(Display.CMD_SET_COLOR, _bytes(color))

    def set_background(self, background):
        self._cmd(Display.CMD_SET_BACKGROUND, _bytes(background))

    def set_font(self, font):
        self._cmd(Display.CMD_SET_FONT, font)

    def start_bootloader(self):
        self._cmd(Display.CMD_START_BOOTLOADER)

    def draw_pixel(self, x, y):
        self._cmd(Display.CMD_DRAW_PIXEL, x, y)

    def draw_line_by(self, x0, y0, x1, y1, color):
        self._cmd(Display.CMD_DRAW_LINE, x0, y0, x1, y1, _bytes(color))

    def draw_line(self, x0, y0, x1, y1):
        self._cmd(Display.CMD_DRAW_LINE_DEFAULT, x0, y0, x1, y1)

    def draw_line_to(self, x, y):
        self._cmd(Display.CMD_DRAW_LINE_TO, x, y)

    def draw_rect_by(self, x, y, w, h, color):
        self._cmd(Display.CMD_DRAW_RECT, x, y, w, h, _bytes(color))

    def draw_rect(self, x, y, w, h):
        self._cmd(Display.CMD_DRAW_RECT_DEFAULT, x, y, w, h)

    def draw_rect_thick(self, x, y, w, h, tx, ty):
        self._cmd(Display.CMD_DRAW_RECT_THICK_DEFAULT, x, y, w, h, tx, ty)

    def draw_rect_thick_by(self, x, y, w, h, tx, ty, color):
        self._cmd(Display.CMD_DRAW_RECT_THICK, x, y, w, h, tx, ty, _bytes(color))

    def fill_screen(self, color):
        self._cmd(Display.CMD_FILL_SCREEN, _bytes(color))

    def fill_rect_by(self, x, y, w, h, color):
        self._cmd(Display.CMD_FILL_RECT, x, y, w, h, _bytes(color))

    def fill_rect(self, x, y, w, h):
        self._cmd(Display.CMD_FILL_RECT_DEFAULT, x, y, w, h)        

    def draw_circle_by(self, x, y, r, color):
        self._cmd(Display.CMD_DRAW_CIRCLE, x, y, r, _bytes(color))

    def draw_circle(self, x, y, r):
        self._cmd(Display.CMD_DRAW_CIRCLE_DEFAULT, x, y, r)

    def fill_circle_by(self, x, y, r, color):
        self._cmd(Display.CMD_FILL_CIRCLE, x, y, r, _bytes(color))

    def fill_circle(self, x, y, r):
        self._cmd(Display.CMD_FILL_CIRCLE_DEFAULT, x, y, r)

    def draw_char(self, ch):
        self._cmd(Display.CMD_DRAW_CHAR, ch)

    def draw_char_xy(self, x, y, ch):
        self._cmd(Display.CMD_DRAW_CHAR_XY, x, y, ch)

    def fill_char(self, ch):
        self._cmd(Display.CMD_FILL_CHAR, ch)

    def fill_char_xy(self, x, y, ch):
        self._cmd(Display.CMD_FILL_CHAR_XY, x, y, ch)

    def draw_string(self, s):
        self._cmd(Display.CMD_DRAW_STRING, s)

    def draw_string_xy(self, x, y, s, anchor):
        self._cmd(Display.CMD_DRAW_STRING_XY, x, y, anchor, s)

    def fill_string(self, s):
        self._cmd(Display.CMD_FILL_STRING, s)

    def fill_string_xy(self, x, y, s, anchor):
        self._cmd(Display.CMD_FILL_STRING_XY, x, y, anchor, s)

    def draw_vertical_scrollbar(self, x, y, w, h, index, items, interval):
        self._cmd(Display.CMD_DRAW_VERTICAL_SCROLLBAR, x, y, w, h, _bytes(index), _bytes(items), interval)

    def draw_progress_bar(self, x, y, w, h, progress):
        self._cmd(Display.CMD_DRAW_PROGRESSBAR, x, y, w, h, progress)
        


    def debug(self):
        self._cmd(Display.CMD_DEBUG)
        res = ''
        while True:
            c = self.serial.read()
            res += c
            if len(c) == 0:
                break
        print res


    def _read_all(self):
        while True:
            try:
                self.serial.read()
                return
            except:
                return

    def _cmd(self, *args):
        if self.prefix >= 0:
            self._write(self.prefix)
            self._write(len(args))
        for a in args:
            #print a
            if type(a) is tuple:
                for v in a:
                    self._write(v)
            elif type(a) is str:
                for c in a:
                    self._write(ord(c))
                self._write(0)
            else:
                self._write(a)

    def _write(self, b):
        #print '>> ', b
        if self.serial is not None:
            self.serial.write(chr(b))
        else:
            self.sock.send(chr(b))

    def _read(self):
        if self.serial is not None:
            b = ord(self.serial.read())
        else:
            b = ord(self.sock.recv(1))
        #print '<< ', b
        return b




    @staticmethod
    def color(r, g, b):
        # rrrrrggg:gggbbbbb
        if isinstance(r, float) or isinstance(g, float) or isinstance(b, float):
            if r > 1:
                r = 1.0
            if g > 1:
                g = 1.0
            if b > 1:
                b = 1.0
            r = int(0x1f * r) & 0x1f
            g = int(0x3f * g) & 0x3f
            b = int(0x1f * b) & 0x1f
        else:
            r = (0x1f * r / 0xff) & 0x1f
            g = (0x3f * g / 0xff) & 0x3f
            b = (0x1f * b / 0xff) & 0x1f

        result = b & 0x1f
        result += ((g & 0x3f) << 5)
        result += (r & 0x1f) << 11

        return result
