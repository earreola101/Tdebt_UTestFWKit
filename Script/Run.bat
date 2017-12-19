@echo off
SET PATH=%PATH%;"C:\Python27\";"C:\Jenkins\TDEBT_CMD_TEST\";"C:\Program Files\Git\";"C:\Program Files (x86)\Git\"
C:\Python27\python.exe "Run_HID_TESTS.py"
exit %errorlevel%