package ru.trolsoft.display128x128;

import java.io.IOException;
import java.io.UnsupportedEncodingException;

/**
 * @author trol
 * Created on 02/05/17.
 */
public class Display implements DisplayCommands {

    public static final int KEY_UP = 0;
    public static final int KEY_DOWN = 1;
    public static final int KEY_LEFT = 2;
    public static final int KEY_RIGHT = 3;
    public static final int KEY_ENTER = 4;


    public static final int WIDTH = 128;
    public static final int HEIGHT = 128;


    public static final int ANCHOR_LEFT = 0;
    public static final int ANCHOR_RIGHT = 1;
    public static final int ANCHOR_CENTER_H = 2;
    public static final int ANCHOR_TOP = 0;
    public static final int ANCHOR_BOTTOM = 4;
    public static final int ANCHOR_CENTER_V = 8;
    public static final int ANCHOR_FULL_LINE = 16;
    public static final int ANCHOR_AUTO_WRAP = 32;
    public static final int ANCHOR_CENTER = ANCHOR_CENTER_V | ANCHOR_CENTER_H;

    public static final int COLOR_BLACK = 0x0000;
    public static final int COLOR_BLUE = 0x001F;
    public static final int COLOR_RED = 0xF800;
    public static final int COLOR_GREEN = 0x07E0;
    public static final int COLOR_CYAN = 0x07FF;
    public static final int COLOR_MAGENTA = 0xF81F;
    public static final int COLOR_YELLOW = 0xFFE0;
    public static final int COLOR_WHITE = 0xFFFF;


    private final DeviceInterface deviceInterface;

    public Display(DeviceInterface deviceInterface) {
        this.deviceInterface = deviceInterface;
    }

    public int sync(int v) throws IOException {
        write(CMD_SYNC, v);
        return read();
    }

    public void getVersion() throws IOException {
        write(CMD_GET_VERSION);
        // TODO
    }

    public int[] readKeybaord() throws IOException {
        write(CMD_READ_KEYBOARD);
        int mask = read();
        if (mask == 0) {
            return null;
        }
        int[] result = new int[5];
        for (int i = 0; i < result.length; i++) {
            result[i] = (mask & (1 << i)) != 0 ? read() : 0;
        }
        return result;
    }

    public void setBacklightTime(int val16) throws IOException {
        write(CMD_SET_BACKLIGHT_TIME);
        writeWord(val16);
    }

    public void setBacklightLevelHigh(int val8) throws IOException {
        write(CMD_SET_BACKLIGHT_LEVEL_HIGH, val8);
    }

    public void setBacklightLevelLow(int val8) throws IOException {
        write(CMD_SET_BACKLIGHT_LEVEL_LOW, val8);
    }

    public void setKeyboardBeep(boolean enable) throws IOException {
        write(CMD_SET_KEYBOARD_BEEP, enable ? 1 : 0);
    }

    public void setBeeperVolume(int val8) throws IOException {
        write(CMD_SET_BEEPER_VOLUME, val8);
    }

    public void beep(int freq16, int duration8) throws IOException {
        write(CMD_BEEP);
        writeWord(freq16);
        write(duration8);
    }


    public void playMelody() {
        // TODO
    }

    public void setStringIntervalX(int interval8) throws IOException {
        write(CMD_SET_STRING_INTERVAL_X, interval8);
    }

    public void setStringIntervalY(int interval8) throws IOException {
        write(CMD_SET_STRING_INTERVAL_Y, interval8);
    }

    public void setColor(int color16) throws IOException {
        write(CMD_SET_COLOR);
        writeWord(color16);
    }

    public void setBackground(int color16) throws IOException {
        write(CMD_SET_BACKGROUND);
        writeWord(color16);
    }

    public void setFont(int font8) throws IOException {
        write(CMD_SET_FONT);
        writeWord(font8);
    }

    public void startBootloader() throws IOException {
        write(CMD_START_BOOTLOADER);
    }

    public void fillScreen(int color16) throws IOException {
        write(CMD_FILL_SCREEN);
        writeWord(color16);
    }

    public void fillRectBy(int x, int y, int w, int h, int color16) throws IOException {
        write(CMD_FILL_RECT, x, y, w, h);
        writeWord(color16);
    }

    public void fillRect(int x, int y, int w, int h) throws IOException {
        write(CMD_FILL_RECT, x, y, w, h);
    }

    public void drawPixel(int x, int y) throws IOException {
        write(CMD_DRAW_PIXEL, x, y);
    }

    public void drawLineBy(int x0, int y0, int x1, int y1, int color16) throws IOException {
        write(CMD_DRAW_LINE, x0, y0, x1, y1);
        writeWord(color16);
    }

    public void drawLine(int x0, int y0, int x1, int y1) throws IOException {
        write(CMD_DRAW_LINE_DEFAULT, x0, y0, x1, y1);
    }

    public void drawLineTo(int x, int y) throws IOException {
        write(CMD_DRAW_LINE_TO, x, y);
    }

    public void drawRectBy(int x, int y, int w, int h, int color16) throws IOException {
        write(CMD_DRAW_RECT, x, y, w, h);
        writeWord(color16);
    }

    public void drawRect(int x, int y, int w, int h) throws IOException {
        write(CMD_DRAW_RECT_DEFAULT, x, y, w, h);
    }

    public void drawRectThickBy(int x, int y, int w, int h, int tx, int ty, int color16) throws IOException {
        write(CMD_DRAW_RECT_THICK, x, y, w, h, tx, ty);
        writeWord(color16);
    }

    public void drawRectThick(int x, int y, int w, int h, int tx, int ty) throws IOException {
        write(CMD_DRAW_RECT_THICK_DEFAULT, x, y, w, h, tx, ty);
    }

    public void drawCircleBy(int x, int y, int r, int color16) throws IOException {
        write(CMD_DRAW_CIRCLE, x, y, r);
        writeWord(color16);
    }

    public void drawCircle(int x, int y, int r) throws IOException {
        write(CMD_DRAW_CIRCLE_DEFAULT, x, y, r);
    }

    public void fillCircleBy(int x, int y, int r, int color16) throws IOException {
        write(CMD_FILL_CIRCLE, x, y, r);
        writeWord(color16);
    }

    public void fillCircle(int x, int y, int r) throws IOException {
        write(CMD_FILL_CIRCLE_DEFAULT, x, y, r);
    }

    public void drawChar(char ch) throws IOException {
        write(CMD_DRAW_CHAR);
        writeChar(ch);
    }

    public void drawCharXY(int x, int y, char ch) throws IOException {
        write(CMD_DRAW_CHAR_XY, x, y);
        writeChar(ch);
    }

    public void drawStr(String s) throws IOException {
        write(CMD_DRAW_STRING);
        writeStr(s);
    }

    public void drawStrXY(String s, int x, int y, int anchor) throws IOException {
        write(CMD_DRAW_STRING_XY, x, y, anchor);
        writeStr(s);
    }

    public void fillChar(char ch) throws IOException {
        write(CMD_FILL_CHAR);
        writeChar(ch);
    }

    public void fillCharXY(int x, int y, char ch) throws IOException {
        write(CMD_FILL_CHAR_XY, x, y);
        writeChar(ch);
    }

    public void fillStr(String s) throws IOException {
        write(CMD_FILL_STRING);
        writeStr(s);
    }

    public void fillStrXY(String s, int x, int y, int anchor) throws IOException {
        write(CMD_FILL_STRING_XY, x, y, anchor);
        writeStr(s);
    }

    public void drawProgressBar(int x, int y, int w, int h, int progress) throws IOException {
        write(CMD_DRAW_PROGRESSBAR, x, y, w, h, progress);
    }

    public void drawVerticalScrollBar(int x, int y, int w, int h, int index16, int items16, int interval) throws IOException {
        write(CMD_DRAW_VERTICAL_SCROLLBAR, x, y, w, h);
        writeWord(index16);
        writeWord(items16);
        write(interval);
    }





    private static String byteToUnicodeChar(int b) {
        byte[] data = new byte[1];
        data[0] = (byte)b;
        try {
            return new String(data, "windows-1251");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return " ";
        }
    }

    protected void write(int... args) throws IOException {
        if (args.length == 1) {
            deviceInterface.writeByte(args[0]);
        } else {
            deviceInterface.writeBytes(args);
        }
    }

    protected void writeWord(int val16) throws IOException {
        write((val16 >> 8) & 0xff, val16 & 0xff);
    }

    private void writeChar(char ch) throws IOException {
        write(ch);
        // TODO russian in 1251
    }

    private void writeStr(String s) throws IOException {
        write(s.length());
        for (int i = 0; i < s.length(); i++) {
            writeChar(s.charAt(i));
        }
    }

    protected int read() throws IOException {
        return deviceInterface.readByte();
    }

}
