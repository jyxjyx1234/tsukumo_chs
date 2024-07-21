copy ..\tsukumo_hook\Release\*.dll dlls\
copy ..\README.md .\
del release.rar
rar a release.rar trans.dat
rar a release.rar tsukumo_font.ttf
rar a release.rar README.md 
rar a release.rar dlls\*
rar a release.rar image
pause