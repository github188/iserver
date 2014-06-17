color 0b
echo off
cls

set lib_path=.\lib
if exist "%lib_path%" (echo.) else (md "%lib_path%")

set file=.\message.proto
echo on
protoc --cpp_out="%lib_path%" "%file%"

pause