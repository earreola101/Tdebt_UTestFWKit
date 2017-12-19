import subprocess
import sys,string,os
import serial
import random
import unittest
import xmlrunner
from timeit import default_timer
from time import sleep

#automated TDE mode test version 0.1.0

tdebt="./tdebt_148.exe"

class TDE_Modes(unittest.TestCase):
    
    #test_SetMacaddressBT
    def test_SetMacaddressBT(self):
        print("TEST_tdebt_SetMacaddress ")
        cmdStr=tdebt+" -s macaddress 001122334455"
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        r = r.replace("\r\n", " ")
        r = r.replace(" ", " ")
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual(" MacAddress=001122334455[OK]",r)
        return

        
    #test_GetMacaddressBT
    def test_GetMacaddressBT(self):
        print("TEST GetMacaddress")
        cmdStr=tdebt+" -g macaddress "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("001122334455",r)
        return
 
 
    #test_SetMacaddressCentral
    def test_SetMacaddressCentral(self):
        print("TEST SetMacaddressCentral ")
        cmdStr=tdebt+" -s macaddress_central 001122334455"
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        r = r.replace("\r\n", " ")
        r = r.replace(" ", " ")
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual(" MacAddressCentral=001122334455[OK]",r)
        return
        
        
    #test_GetMacaddressCentral
    def test_GetMacaddressCentral(self):
        print("TEST GetMacaddressCentral")
        cmdStr=tdebt+" -g macaddress_central "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("001122334455",r)
        return        
 
    #test_Setmacaddress_peripheral
    def test_SetMacaddressPeripheral(self):
        print("TEST SetMacaddressPeripheral ")
        cmdStr=tdebt+" -s macaddress_peripheral 001122334455"
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        r = r.replace("\r\n", " ")
        r = r.replace(" ", " ")
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual(" MacAddress_Peripheral=001122334455[OK]",r)
        return

    #test_GetDeviceName
    def test_GetDeviceName(self):
        print("Start test_GetDeviceName")
        cmdStr=tdebt+" -g devicename"
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("Logitech MeetUp Speakerphone",r)
        return
        
    #test_GetDeviceNameBLE
    def test_GetDeviceNameBLE(self):
        print("Start test_GetDeviceNameBLE")
        cmdStr=tdebt+" -g devicenameble "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("NB-LRUIW7",r)
        return
        
        
     #test_GetFWVersion
    def test_GetFWVersion(self):
        print("Start test_test_GetFWVersion")
        cmdStr=tdebt+" -g fwversion "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("255",r)
        return
 
     #test_GetHWVersion
    def test_GetFWVersion(self):
        print("Start test_GetFWVersion")
        cmdStr=tdebt+" -g hwversion "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("0",r)
        return 
 
     #test_GetSerialnumber
    def test_GetFWVersion(self):
        print("Start test_GetSerialnumber")
        cmdStr=tdebt+" -g serialnumber "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("30783035653638323830000000",r)
        return
        
      #test_Getbtusbpidvid
    def test_Getbtusbpidvid(self):
        print("Start test_Getbtusbpidvid")
        cmdStr=tdebt+" -g btusbpidvid "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("[M]:VID=0x46d, PID=0x867",r)
        return       
 
      #test_Getbt_nfcstatus
    def test_Getbt_nfcstatus(self):
        print("Start test_Getbt_nfcstatus")
        cmdStr=tdebt+" -g bt_nfcstatus "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("0",r)
        return     
    
      #test_Getbt_nfcstatus
    def test_Getbt_nfcstatus(self):
        print("Start test_Getbt_nfcstatus")
        cmdStr=tdebt+" -g bt_nfcstatus "
        r=subprocess.check_output(cmdStr,stderr=subprocess.STDOUT)
        print("OUTPUT: "+r+"\r\n")
        self.assertEqual("0",r)
        return         
          
        
if __name__ == '__main__':
    with open('results.xml', 'wb') as output:
        unittest.main(
            testRunner=xmlrunner.XMLTestRunner(output=output),
            failfast=False, buffer=False, catchbreak=False)



