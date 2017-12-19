@echo off
SET PATH=%PATH%;"C:\Python27\";"C:\Jenkins\TDEBT_CMD_TEST\";"C:\Program Files\Git\";"C:\Program Files (x86)\Git\"
SET ReleasePath="C:\TDE_DVE\TDEBT_KIT\TDEBT\Release\tdebt.exe"
SET LocalPath="./tdebt.exe"
SET ResultsFile="results.xml"

copy %ReleasePath% %LocalPath%
C:\Python27\python.exe "Run_HID_TESTS.py"
exit %errorlevel%