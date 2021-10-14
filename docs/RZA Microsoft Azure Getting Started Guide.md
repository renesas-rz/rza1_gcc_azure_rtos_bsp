# RZA Microsoft Azure Getting Started Guide

## Renesas RZA1 BSP Board Configuration

The rz_bsp can be configured for different RZA MPUs, Renesas RZA evaluation boards, and GUI drivers. The file that contains these configurations is called "mcu_board_select.h". It can be found in the rz_bsp library under the directory "src/renesas/application/inc/" . The configuration allows selection for the board ( Stream-it or RSK (Display-it kit)), MPU ( RZA1H, M,L,LU,and LC) and GUIX Driver ( 16bit, 24bit, and 32bit ). Once these changes are made, a full rebuild of the rz_bsp is required. By default are below.

```
/* User Select macro */
#define TARGET_RZA1			   (TARGET_RZA1LU)
#define TARGET_BOARD		 (TARGET_BOARD_STREAM_IT2)
#define TARGET_GUI_BPP		   (TARGET_GUI_NONE)
```

*Here the  TARGET_RZA1 is the RZA1LU. The TARGET_BOARD is the Stream-it board. The TARGET_GUI_BPP is set to none.* 

The appropriate settings for the Renesas Evaluation boards are.

| TARGET_BOARD                         | TARGET_RZA1   | TARGET_GUI_RGB565 | TARGET_GUI_RGB888 | TARGET_GUI_RGB8888 |
| ------------------------------------ | ------------- | ----------------- | ----------------- | ------------------ |
| TARGET_BOARD_RSK   ( Display-it Kit) | TARGET_RZA1H  | Yes ( no demo )   | Yes               | Yes ( no demo)     |
| TARGET_BOARD_STREAM_IT2              | TARGET_RZA1LU | Yes               | No[^1]            | No[^1]             |

1) *These supported settings are due to target mpu board design*

## RZA1 Smart Configuration Code Generation

The RZA1 Smart Configurator is a separate PC application software, it is designed for hardware engineers to resolve pin conflicts for the RZA1 processors. The RZA1 SC does not support code generation. ( NOTE : RZA2M Smart Configurator is a different configurator that is integrated with the e2studio IDE ). This Software design kit includes scripts that generated code based on the exported pin settings. 

## Board Settings

#### Renesas RZA1H Display-it Kit

The Azure Demos are designed to run from Serial Flash

Serial Flash Switch settngs SW6 to OFF, ON, OFF, ON, ON, ON

TTY Support Connect to USB Connector CN18. Set the TTY Baudrate to 115200.

#### Renesas RZA1LU Stream-it Kit

The Azure Demos are designed to run from Serial Flash

No Hardware configuration needed

TTY Support Connect to USB Connector CN10. Set the TTY Baudrate to 115200. This is the same USB connector that powers the board.

## RZA1 Bootloader

The Demos requre the RZA1 Serial flash bootloader. The BSP utils director already includes the pre compiled Serial Flash bootloader binaries for the RZA1H Displayit Board and the RZA1LU Stream-it Board. To install these you will need to install [Segger JLink](https://www.segger.com/downloads/jlink) on you PC. The minimium supported verision is 6.00. 

In the BSP util folder run the batch script. The script will prompt you for which Renesas Evaluation board you have. After selecting the correct board the Segger Jlink will download the appropriate bootloader binary file to the board.

This install only needs to be done once. 

For Custom Boards please download the RZA1 Bootloader from the [Renesas RZ Website](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rz-arm-based-high-end-32-64-bit-mpus). Navigate to the correct RZ Processor (RZA1 L,LC, LU, M, H). The bootloader code is located in the software download.

## Renesas IDE e2Studio 

e2Studio IDE Version 7.8 or higher :
https://www.renesas.com/e2studio .

GNU ARM Embedded toolchain (GCC 6; 6-2016-q4-major or higher version) 
Note: this compiler is part of the IDE installation for RZA. During installation when prompted be sure to select this compiler: 
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

At TTY terminal is needed to view the sample demo output. 

## Demos

#### General Setup

The demo projects are dependent on the  libraries with links to the generated library binaries. So the the projects need to be imported to the same folder as the Microsoft libraries(Threadx, USBx, FileX, NetXDuo, GUIx) and Renesas RZA BSP library. The libraries need to be built first before the sample demos. 

This setup is for sample demos that do not need any additional settings ( sample_threadx, sample_filex_ram_disk).

1) Set the MPU board, and GUI Driver( see [Renesas RZA1 BSP Board Configuration](#Renesas RZA1 BSP Board Configuration) ).
2) Full Build of the rz_bsp project.
3) Set the sample project to match the selection in step 1.
4) Build the project.
5) Download the project.

######  sample_threadx

The threadx demo does print any thing to the TTY. The demo shows how to use the Threadx IPC, task management, and memory management. To examine the demo you can step throught the code with the debugger. 

###### sample_filex_ram_disk

The FileX demo does print any thing to the TTY. The demo shows how to use the File Management API. To examine the demo you can step throught the code with the debugger. 

#### NetXDuo Sample Demos

1) Set the MAC Address in the nx_driver_renesas_rz.h driver. This file is found in the rz_bsp project folder azure/ports/cortex_a9/gnu/inc . Below are the lines that need to be changed. The board MAC address can be found on a label on the Renesas Evaluation board. 

   ```
   /* MAC Address - place lower 4 bytes in _L and upper 2 bytes in _H. */
   #define ETHERNET_MAC_H              0x00000011
   #define ETHERNET_MAC_L              0x22334455
   ```

   

2) Full Build of the rz_bsp project.

3) Set the build configuration for the sample project to match the settings in the "mcu_board_select.h".

4) Clean and build of the NetXDuo sample projects.

5) Download the sample project. Be sure to select the debug configuration that matches the build configuration. 

6. Run the TTY terminal ( PUTTY or Tera Term). Set the baud rate to 115200. When DHCP completes the ipaddress will appear.  For Azure samples additional debug information is displayed.

#### Azure Cloud Setup

   Additional setup is requred for these Azure Sample Demos. Follow the directions for each demos to setup the IoT Connection information. Each of the these documents has links to setup the Azure Portal and registering a device.

- [Sample Azure iot Embedded sdk Readme.md](..\sample_azure_iot_embedded_sdk\src\README.md)
- [Sample Azure iot Embedded sdk pnp Readme.md](..\sample_azure_iot_embedded_sdk_pnp\src\README.md)
- [Sample pnp Temperature Controller Readme.md](..\sample_pnp_temperature_controller\src\README.md)

1. Open the file **sample_config.h** in the demo.  Using the Azure IoT Device info from the portal set the following values.

   ```
   #define HOST_NAME                                   ""
   #define DEVICE_ID                                   ""
   #define DEVICE_SYMMETRIC_KEY                        ""
   #define MODULE_ID                                   ""
   ```

   

   

#### GUIX Sample Demos

1) Set the MPU board, and GUI Driver( see [Renesas RZA1 BSP Board Configuration](#Renesas RZA1 BSP Board Configuration) ).
2) Full Build of the rz_bsp project.
3) The GUIX Sample Demos are designed specifically for a RZA1 MPU and RZ Evaluation board.  Depending on the board used you must pick the corresponding demo. For example demo "rza1lu_sample_guix_home_automation" is designed RZA1LU MPU and the Stream-t Evaluation board.
4) Build the sample demo
5) Download the project.

#### USBX Sample Demos

1. Set the MPU board, and GUI Driver( see [Renesas RZA1 BSP Board Configuration](#Renesas RZA1 BSP Board Configuration) ).

2. Full Build of the rz_bsp project.

3. In the USBX library set the USB channel. For USB Device open "\common\usbx_device_controllers\ux_dcd_rz.h". For USB Host open "\common\usbx_host_controllers\ux_hcd_rz.h"

4. Set the preprocessor define UX_RZ_USB_BASE to the desired channel. Note:  The setting in the file "ux_hcd_rz.h" has condition compilation depending on the board selected, so it may not be required to make additional settings.

   ```
   #define UX_RZ_USB0_BASE                             0xE8010000
   #define UX_RZ_USB1_BASE                             0xE8207000
   ....
   #define UX_RZ_USB_BASE                              UX_RZ_USB0_BASE
   ```

5. Rebuild the USBx library. This is required because the RZ USB drivers are part of the USBX library. These drivers have conditional compilations for the selected board ( not MPU). 

6. In the USB Sample demo select the build configuration that matches the selection in step 1.

7. Download and Run demo. 

   ###### sample_usbx_host_mass_storage

   When this demo runs a USB Mass Storage device needs to be connect to the board. On the TTY the file contents will be printed out.

   ###### sample_usbx_device_mass_storage

   The demo demonstrates the RZA as a USB MS Device. For this demo depending on the board used a USB cable with two type A connectors is needed. Connect one end to a PC and the other to the RZA Board. When the demo runs Windows will open a emtpy File Explorer Window. 

   ## FAQ

   For FAQ and custom board support see.

   [Renesas info RZA1 Microsoft Azure](https://renesas.info/wiki/RZ-A/RZA_AzureRTOS)
