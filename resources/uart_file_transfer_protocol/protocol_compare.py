# coding:utf-8

import serial
from xmodem import XMODEM

ser = serial.Serial('COM6', timeout=0)

def getc(size, timeout=1):
    return ser.read(size) or None


def putc(data, timeout=1):
    return ser.write(data)  # note that this ignores the timeout


def sendFileByXMODEM(file_name):
    modem = XMODEM(getc, putc)
    stream = open(file_name, 'rb')
    modem.send(stream)


def sendFileByYMODEM(file_name):
    pass


def sendFileByZMODEM(file_name):
    pass

def main():
    sendFileByZMODEM('test.txt')

if __name__ == '__main__':
    main()