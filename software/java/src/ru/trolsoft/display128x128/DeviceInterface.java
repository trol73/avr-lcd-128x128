package ru.trolsoft.display128x128;

import java.io.IOException;

/**
 * @author Trol
 * Created on 02/05/17.
 */
public interface DeviceInterface {

    int readByte() throws IOException;
    void writeByte(int v) throws IOException;

    void tryReadAll();

    default int[] readBytes(int cnt) throws IOException {
        int[] result = new int[cnt];
        for (int i = 0; i < cnt; i++) {
            result[i] = readByte();
        }
        return result;
    }

    default void writeBytes(int[] data) throws IOException {
        for (int b : data) {
            writeByte(b);
        }
    }
}
