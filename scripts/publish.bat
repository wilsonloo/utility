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
@echo moving %LIB_DIR%evl_utility_static-rtl_debug.lib to %EVL_PUBLISH_ROOT%third_party\utility\lib\
move %LIB_DIR%evl_utility_static-rtl_debug.lib %EVL_PUBLISH_ROOT%third_party\utility\lib\

@echo moving %LIB_DIR%evl_utility_static-rtl_debug.pdb to %EVL_PUBLISH_ROOT%third_party\utility\lib\
move %LIB_DIR%evl_utility_static-rtl_debug.pdb %EVL_PUBLISH_ROOT%third_party\utility\lib\

@echo moving %LIB_DIR%evl_utility_static-rtl_release.lib to %EVL_PUBLISH_ROOT%third_party\utility\lib\
move %LIB_DIR%evl_utility_static-rtl_release.lib %EVL_PUBLISH_ROOT%third_party\utility\lib\

@echo moving %LIB_DIR%evl_utility_static-rtl_release.pdb to %EVL_PUBLISH_ROOT%third_party\utility\lib\
move %LIB_DIR%evl_utility_static-rtl_release.pdb %EVL_PUBLISH_ROOT%third_party\utility\lib\
@echo copying libraries ... OK

@echo publushing ... OK

pause