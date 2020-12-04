@echo off

set opts=-FC -GR- -EHa- -Od -Oi -MT -TC -Wall -WX -Wno-unused-parameter -Wno-nonportable-system-include-path -nologo -Z7
set code=%cd%
pushd build
clang-cl %opts% %code%\code\main.c -FehomebrewJourney
popd
