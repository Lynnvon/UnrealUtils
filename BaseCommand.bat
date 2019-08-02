@echo off

set CurrentPath=%~dp0

rem ���²��������Լ�ʵ��·�������޸�

set ProjectName=FlightSim
set ProjectDir=%CurrentPath%..
set UATPath="D:\UE4SouceBuild\UnrealEngine-4.21\Engine\Build\BatchFiles\RunUAT.bat"
set AutomationScriptPath="%CurrentPath%AllBuild.xml"

rem ends

if %Target%=="" set Target="Build Editor Win64"
if "%IterativeCooking%"=="" set IterativeCooking=true
if "%BuildConfiguration%"=="" set BuildConfiguration=Development
if "%WithClean%"=="" set WithClean=false

call %UATPath% BuildGraph -Script=%AutomationScriptPath% -Target=%Target% -set:ProjectDir=%ProjectDir% -set:ProjectName=%ProjectName% -set:IterativeCooking=%IterativeCooking% -set:BuildConfiguration=%BuildConfiguration% -set:WithClean=%WithClean%

pause                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        