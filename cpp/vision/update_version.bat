@ECHO OFF

ECHO ==================================================
ECHO ���ɳ�����Ϣ...

SET SOLUTION_DIR=%1
SET WORKING_DIR=%SOLUTION_DIR%
SET SOURCE_VERSION_FILE=%SOLUTION_DIR%/NFComm/Utility/version_base.h
SET DEST_VERSION_FILE=%SOLUTION_DIR%/NFComm/NFPluginLoader/Version.h

ECHO ʹ�ð汾ģ��: %SOURCE_VERSION_FILE%
ECHO ���ɰ汾�ļ�: %DEST_VERSION_FILE%

SubWCRev.exe %WORKING_DIR% %SOURCE_VERSION_FILE% %DEST_VERSION_FILE%

ECHO ���ɳ�����Ϣ...OK



