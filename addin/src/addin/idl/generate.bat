@echo off

SET IDL_DIR=%~dp0
cd /d %IDL_DIR%

midl /out ..\interface   /h msaddndr.h       /notlb msaddndr.idl
midl /out ..\interface   /h ribbon.h         /notlb ribbon.idl
midl /out ..\interface   /h ribboncallback.h /notlb ribboncallback.idl
midl /out ..\typelib                                ribboncallback.idl

