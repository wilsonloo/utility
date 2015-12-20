@echo off
@echo publushing ...

set INCLUDE_DIR=%CD%/../include/
set LIB_DIR=%CD%\..\lib\
set PUBLISH_INCLUDE_DIR=%EVL_PUBLISH_ROOT%third_party\utility\include\
set PUBLISH_LIB_DIR=%EVL_PUBLISH_ROOT%third_party\utility\lib\

@echo copying include files from %INCLUDE_DIR% to %PUBLISH_INCLUDE_DIR%
robocopy /E %INCLUDE_DIR% %PUBLISH_INCLUDE_DIR%
@echo copying include files ... OK

@echo copying libraries ...

@echo moving %LIB_DIR%*.lib to %EVL_PUBLISH_ROOT%third_party\utility\lib\
move %LIB_DIR%*.lib %EVL_PUBLISH_ROOT%third_party\utility\lib\

@echo moving %LIB_DIR%*.pdb to %EVL_PUBLISH_ROOT%third_party\utility\lib\
move %LIB_DIR%*.pdb %EVL_PUBLISH_ROOT%third_party\utility\lib\

@echo copying libraries ... OK

@echo publushing ... OK

pause