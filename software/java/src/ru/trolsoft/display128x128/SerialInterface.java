package ru.trolsoft.display128x128;

import jssc.SerialPort;
import jssc.SerialPortException;
import jssc.SerialPortTimeoutException;

import java.io.IOException;

/**
 * @author Trol
 * Created on 02/05/17.
 */
public class SerialInterface implements DeviceInterface {

    private SerialPort serialPort;

    public SerialInterface(SerialPort serialPort) {
        this.serialPort = serialPort;
    }



    @Override
    public int readByte() throws IOException{
        try {
            return (int)serialPort.readBytes(1)[0];
        } catch (SerialPortException e) {
            throw new IOException(e);
        }
    }

    @Override
    public void writeByte(int v) throws IOException {
        try {
            serialPort.writeByte((byte) v);
        } catch (SerialPortException e) {
            throw new IOException(e);
        }
    }

    @Override
    public void tryReadAll() {
        while (true) {
            try {
                int v = serialPort.readBytes(1, 250)[0];
                System.out.println("try to read all " + v);
            } catch (SerialPortException | SerialPortTimeoutException e) {
                e.printStackTrace();
            }
        }
    }

}
