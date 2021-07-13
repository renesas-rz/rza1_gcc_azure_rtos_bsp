@echo off

:loop
echo Please select one of the following options:
echo. 
echo  1 - RZA1H Displayit (RSK)
echo  2 - RZA1LU Streamit 
echo  3 - RZA1LU Streamit Ethernet (no Display)
echo  4 - Custom Board
echo. 
echo  x - Exit
echo.
set /p SELECTED=Your choice: 

if "%SELECTED%" == "x" goto :eof
if "%SELECTED%" == "1" goto :Displayit
if "%SELECTED%" == "2" goto :Streamit
if "%SELECTED%" == "3" goto :eStreamit
if "%SELECTED%" == "4" goto :custom
goto :errorInput 

:Displayit
echo Building GPIO configuration for the RZA1H Displayit Kit Board
python .\scripts\code_generator.py rza1h_Displayit.xml
move /Y r_gpio_cfg.h ..\..\src\renesas\drivers\r_gpio\inc\r_rza1h_port_sc_cfg.h
move /Y mcu_board_select.h ..\..\src\renesas\application\inc\mcu_board_select.h
goto :loop

:Streamit
echo Building GPIO configuration for the RZA1LU Streamtit Kit Board with LCD support
python .\scripts\code_generator.py rza1lu_Streamit_Display.xml
move /Y r_gpio_cfg.h ..\..\src\renesas\drivers\r_gpio\inc\r_rza1l_port_sc_cfg.h
move /Y mcu_board_select.h ..\..\src\renesas\application\inc\mcu_board_select.h
goto :loop

:eStreamit
echo Building GPIO configuration for the RZA1LU Streamtit Kit Board with Ethernet Support
python .\scripts\code_generator.py rza1lu_Streamit_Ethernet.xml
move /Y r_gpio_cfg.h ..\..\src\renesas\drivers\r_gpio\inc\r_rza1l_port_sc_cfg.h
move /Y mcu_board_select.h ..\..\src\renesas\application\inc\mcu_board_select.h
goto :loop

:custom
echo Currently not supported
goto :loop
