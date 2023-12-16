echo WScript.sleep 500 >sss.vbs

start ARPGdbServer.bat
sss.vbs
start ARPGCenterServer.bat
sss.vbs
start ARPGGateServer.bat
sss.vbs
start ARPGLoginServer.bat