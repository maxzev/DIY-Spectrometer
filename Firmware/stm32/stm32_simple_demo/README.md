Simple demo for stm32 Nucleo-64 board.
Reads values from spectrometer and displays the spectrum on the TFT screen every 5 seconds.

GNU Lesser General Public License v3.0
Copyrights by Max Zev max_zev@protonmail.com
https://github.com/maxzev/DIY_Spectrometer

Credits for ILI9341 display to https://github.com/afiskon/stm32-ili9341

Tested with Nucleo-64 stm32L476RG.

Setting up with STM32L4xx:
  - Extract the project into a folder and run STM32CubeIDE
  - From STM32CubeIDE: File->New->STM32 Project from Existing STM32CubeMx Configuration File (.ioc) 
  - Add folders ILI9341_display and Spectrometer to the project. Right click on project name in Project Explorer ->Refresh.
  - Add ILI9341_display and Spectrometer folders to Properties->C/C++General->Paths & Symbols (for "Includes" and "Source Location")
  - Copy the content of stm32_simple_demo/Core/Src/main.c to main.c of the new project. 
