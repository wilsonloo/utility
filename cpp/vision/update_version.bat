@ECHO OFF

ECHO ==================================================
ECHO 生成程序集信息...

SET SOLUTION_DIR=%1
SET WORKING_DIR=%SOLUTION_DIR%
SET SOURCE_VERSION_FILE=%SOLUTION_DIR%/NFComm/Utility/version_base.h
SET DEST_VERSION_FILE=%SOLUTION_DIR%/NFComm/NFPluginLoader/Version.h

ECHO 使用版本模板: %SOURCE_VERSION_FILE%
ECHO 生成版本文件: %DEST_VERSION_FILE%

SubWCRev.exe %WORKING_DIR% %SOURCE_VERSION_FILE% %DEST_VERSION_FILE%

ECHO 生成程序集信息...OK



