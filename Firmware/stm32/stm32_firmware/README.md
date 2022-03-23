Firmware for stm32F401 board.

GNU Lesser General Public License v3.0
Copyrights by Max Zev max_zev@protonmail.com
https://github.com/maxzev/DIY_Spectrometer

Credits for ILI9341 display to https://github.com/afiskon/stm32-ili9341

Tested with Nucleo-64 stm32L476RG, stm32F103, stm32F401.

Setting up with stm32F401, option1:
  - Extract the project into a folder and run STM32CubeIDE
  - From STM32CubeIDE: File->New->STM32 Project from Existing STM32CubeMx Configuration File (.ioc) 
  - Add folders Gui, ILI9341_display, Spectrometer to the project folder. Right click on project name in Project Explorer ->Refresh.
  - Add folders Gui, ILI9341_display, Spectrometer to Properties->C/C++General->Paths & Symbols (for "Includes" and "Source Location")
  - Copy the content of spectrometer/Core/Src/main.c to main.c of the new project. 


TODO list:
1) Buttons
2) Settings menu (Integration time adjustment, Dark scan, etc)
3) Save data to SD card
4) Save current graph (photo)
5) Connection to PC

