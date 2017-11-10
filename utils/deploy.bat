rem D:\user\Bertrand\Documents\GitHub\Peezer\utils\deploy.bat  "${ProjDirPath}/Debug/${ProjName}" /home/pi/projects/${ProjName}.a
ECHO OFF

ECHO Start deploy

rem Check for a valid filename
IF "%1"=="" (
	ECHO You have to provide a valid source file.
	EXIT /b
)

IF "%2"=="" (
	ECHO You have to provide a valid destination path.
	EXIT /b
)


 

SET PUTTYSCP_BIN="C:\Program Files (x86)\WinSCP\pscp.exe"
SET PUTTY_BIN="C:\Program Files (x86)\WinSCP\putty.exe"

rem SET RASPBERRYPI_ADDR=BoomV
SET RASPBERRYPI_ADDR=Peezer
rem SET RASPBERRYPI_ADDR=BoomB
rem SET RASPBERRYPI_ADDR=BoomM
SET USERNAME=pi

SET PASSWORD=raspberry
SET CMD_FILENAME=commands.sh

rem Upload the file to raspberry pi
%PUTTYSCP_BIN% -pw %PASSWORD% "%1" %USERNAME%@%RASPBERRYPI_ADDR%:"%2"

rem Build a list of actions to do on the pi (chmod, execute GDB server)
rem if exist %~dp0%CMD_FILENAME% del %~dp0%CMD_FILENAME%
rem echo rm "%2" >> %~dp0%CMD_FILENAME%
rem echo chmod +x "%2" >> %~dp0%CMD_FILENAME%
rem echo gdbserver :3785 "%2" >> %~dp0%CMD_FILENAME%

rem Execute the action list on the raspberry pi
rem %PUTTY_BIN% -pw %PASSWORD% -m %~dp0%CMD_FILENAME% %USERNAME%@%RASPBERRYPI_ADDR%
 
rem exit /b %ERRORLEVEL%

