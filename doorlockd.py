#!/usr/bin/env python
# Configuration, must match the configuration in doorlockd.py

port = "/dev/ttyUSB2"   # ttyUSB port to connect to
SerialOpen  = 0x05      # Byte to open the door
SerialAck   = 0x06      # Byte received when acknowledging receipt
SerialNack  = 0x15      # Byte received when Arduino cannot translate the packet
SerialOk    = 0x11      # Byte received when the door opened successfully
SerialNo    = 0x12      # Byte received when the door could not open

import serial                     #Using pySerial
sta = serial.Serial(port, 9600)   #TTY Port Connection

while(1):
    print ("1] Open Door\n")
    c = raw_input("> ")
    c = int(c)
    if(c==1):
        sta.write(SerialOpen)
        RCPT = sta.read()
        if ( RCPT == SerialAck ):
            print "Arduino Acknowledged Receipt"
            RCPT = sta.read()
            if ( RCPT == SerialOk ):
                print "The door is allegedly open"
            elif ( RCPT == SerialNo ):
                print "Arduino could not open the door"
            else:
                print "Arduino Protocol Mismatch"
        elif ( RCPT == SerialNack ):
            print "Arduino reported decoding issue"
        else:
            print "Arduino does not reply. (Protocol mismatch)"
