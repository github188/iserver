echo off

rem set vs2008 cli environment
call "%VS90COMNTOOLS%VSVARS32.BAT"

rem ================== Common =========================
vcbuild /rebuild .\libprotocol.sln "Debug|Win32"

rem vcbuild /rebuild .\libprotocol.sln "Release|Win32"