# coding:utf-8

import time
import serial
from xmodem import XMODEM

ser = serial.Serial('COM6', 115200, timeout=0.5)

def getc(size, timeout=1):
    return ser.read(size) or None


def putc(data, timeout=1):
    return ser.write(data)  # note that this ignores the timeout


def sendFileByXMODEM(file_name):
    print("----------- Start -----------")
    time1 = time.time()
    modem = XMODEM(getc, putc)
    stream = open(file_name, 'rb')
    modem.send(stream)
    stream.close()
    ser.close()
    time2 = time.time()
    print("End: ",time2-time1)


def main():
    print(ser.baudrate)
    sendFileByXMODEM('test.jpg')


if __name__ == '__main__':
    main()