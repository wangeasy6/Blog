# coding:utf-8

import serial
from xmodem import XMODEM

ser = serial.Serial('COM10', 115200, timeout=0.5)

def getc(size, timeout=1):
    return ser.read(size) or None


def putc(data, timeout=1):
    return ser.write(data)  # note that this ignores the timeout


def recvFileByXMODEM(file_name):
    modem = XMODEM(getc, putc)
    stream = open(file_name, 'wb')
    modem.recv(stream)
    stream.close()
    ser.close()


def main():
    recvFileByXMODEM('test_recv.jpg')


if __name__ == '__main__':
    main()