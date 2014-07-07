# Use Pythone 3.3
# Send Binary Code To STM32 (In-Application Programming)
# -*- coding: UTF-8 -*-

import serial


def RS232_ReadBinFile(FilePath):
    file = open(FilePath, "rb")
    try:
        ReadData = file.read()
    finally:
        file.close()

    PrintStr = "\n"
    PrintStr += (" File Size : " + str(len(ReadData)) + " ( Bytes )\n")
    print(PrintStr)

    PrintStr = "\n Binary Code ( Hex ) : \n"
    PrintStr += " -------------------------------------------------------- \n"

    DataSize = len(ReadData)
    TableRow = DataSize>>4
    for i in range(0, TableRow+1) :
        PrintStr += (" " + format(i+1, '5d') + "   ")

        Data = (DataSize - i*16) >> 4
        if Data != 0 :
            dataCol = 16
        else :
            dataCol = DataSize - i*16

        for j in range(dataCol) :
            strTemp = format(ReadData[(i*16)+j], '2x')
            if strTemp[0] == ' ' :
                PrintStr += ('0' + strTemp[1])
            else :
                PrintStr += (strTemp)
            if j%2 == 1 :
                PrintStr += ("  ")
        PrintStr += "\n"
    PrintStr += " -------------------------------------------------------- \n"

#    print(PrintStr)

    return ReadData


def RS232_SendCodeSize(BinCodeSize):
    device.flushInput()

    SendCodeSize = []
    for i in range(0, 4) :
        Sfift = ((3-i)<<3)
        TempData = (BinCodeSize & 0xFF<<Sfift) >> Sfift
        SendCodeSize.append(TempData)
        device.write(SendCodeSize[i].to_bytes(1, 'big'))
    RecvData = device.read(4)

    RecvCodeSize = (RecvData[0]<<24) | (RecvData[1]<<16) | (RecvData[2]<<8) | (RecvData[3])
    if RecvCodeSize == BinCodeSize :
        device.write((0xF0).to_bytes(1, 'big'))
    else :
        device.write((0x0F).to_bytes(1, 'big'))
    PrintStr = (" RecvCodeSize = %d\n" % RecvCodeSize)
    print(PrintStr)

    return RecvCodeSize


def RS232_Send_Data(SendData, DataLen):
    device.flushInput()
    for i in range(0, DataLen) :
        device.write(SendData[i].to_bytes(1, 'big'))


def RS232_Send_File(SendFile):
    device.flushInput()

    WriteCnt = 0
    FileSize = len(SendFile)

    ErrCnt = 0
    PrintStr = " ErrCnt = "
    for i in range(0, FileSize>>10) :
        RS232_Send_Data(SendData[WriteCnt:WriteCnt+1024], 1024)
        RecvData = device.read(1024)
        for j in range(0, 1024) :
            if(RecvData[j] != SendData[WriteCnt+j]) :
                ErrCnt += 1
        PrintStr += (str(ErrCnt) + ", ")
        WriteCnt += 1024
    TempSize = FileSize - WriteCnt
    if TempSize != 0 :
        RS232_Send_Data(SendData[WriteCnt:FileSize], TempSize)
        RecvData = device.read(TempSize)
        for j in range(0, TempSize) :
            if(RecvData[j] != SendData[WriteCnt+j]) :
                ErrCnt += 1
        PrintStr += (str(ErrCnt) + "\n")
    print(PrintStr)


def RS232_Send_FileNoRecv(SendFile):
    device.flushInput()

    WriteCnt = 0
    FileSize = len(SendFile)

    for i in range(0, FileSize>>10) :
        RS232_Send_Data(SendData[WriteCnt:WriteCnt+1024], 1024)
        WriteCnt += 1024
    TempSize = FileSize - WriteCnt
    if TempSize != 0 :
        RS232_Send_Data(SendData[WriteCnt:FileSize], TempSize)


device = serial.Serial()
device.port = 'COM1'
device.baudrate = 115200
device.bytesize = 8
device.stopbits = 1
device.parity = 'N'
device.xonxoff = False
device.timeout = 10
device.open()

SendData = RS232_ReadBinFile("STM32F4_QFC_IAP-TestLED.bin")
RS232_SendCodeSize(len(SendData))
RS232_Send_File(SendData)

device.close()
