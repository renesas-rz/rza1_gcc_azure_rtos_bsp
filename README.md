# RZA1 Microsoft Azure SDK

This is the RZA1 Software Development with Microsoft Azure Support. The SDK contains the RZA1 Board Support Package and drivers for the RZA1 LU, LC, L, M, and H target Processors, the Asure RTOS ( Threadx), Azure middleware ( USB Host, USB Device, Network, IoT, Filesystem, GUI Development). In driver GPIO code generation has been integrated with the RZA1 Smart Configurator.

## Microsoft Azure Libraries

The [Microsoft Azure software](https://azure.microsoft.com/en-us/free/search/?&ef_id=EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE:G:s&OCID=AID2100131_SEM_EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE:G:s&gclid=EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE) is supported by the Azure Microsoft Team.  This repository includes the library debug binaries. The full source code for the listed libraries can be found [here](https://github.com/azure-rtos/samples).

License

- **Threadx** 

  This is the Microsoft Azure RTOS

- **USBx**

  This is the USB middleware. It support both Host and Device. USB Class support include HID, MSC, CDC,   HUB, UVC, Audio, and printer.

- **GUIx**

  This the GUI middleware. It supports 16, 24 and 32 bit color space. Resolution is dependent on the amount of internal memory ( i.e. 2MB RZA1LC - 10MB RZA1H ) or external memory. 

- **FileX**

  This is the RTOS file system. Supports RAM disk, USB and SD ( requires request form Renesas ).

- **NetXDuo**

  This middware supplies the network for IP4, Ip6, TCP, UDP, HTTP, Secure Socket Layer, Crypto libraries, and many more.

- **Azure IoT**

  As part of the NetxDuo the Azure IoT Cloud connectivity is included. 

## Microsoft Azure Demos

- **sample_threadx**

  This is a simple demo of the basic threadx RTOS APIs and memory management.

- **r_sample_threadx**

  This demo is similar to the sample_threadx but uses the Renesas OS Abstraction Layer

- **sample_filex_ramdisk**

  This project demonstrates the FileX API for a mass storage device located on the RZA internal RAM.

- **sample_netx_duo_iperf**

  This project demonstrates the NetX API running a webpage that shows the network connectivity performance.

- **sample_netx_duo_ping**

  This project demonstrates the NetX Duo API that responds to ping.

- **sample_azure_iot_embedded_sdk**

  This project demonstrates the Azure IoT. A license is required to access the Microsoft Azure Cloud.

- **sample_azure_iot_sdk_ping**

  This project demonstrates the Azure IoT. A license is required to access the Microsoft Azure Cloud.

- **rza1h_sample_guix_home_automation**

  This project demonstrates the GUIX API. The demo simulates a home automation HMI controller. This includes lights, HVAC, Door Locks, and Outdoor Weather. The project is designed for the RZA1H RSK board. The resolution is 800 x 600 with 24bpp color depth.

## Renesas RZA1 BSP Board Configuration

The rz_bsp has Renesas Evaluation board and MPU settings. The file is called "mcu_board_select.h". It can be found in the rz_bsp library in the "src/renesas/application/inc" folder. The configuration allows selection for the board ( Stream-it or RSK (Display-it kit)), MPU ( RZA1H, M,L,LU,and LC) and GUIX Driver ( 16bit, 24bit, and 32bit ). Once these changes are made, a full rebuild of the rz_bsp is required.

## RZA1 Smart Configuration Code Generation

The RZA1 Smart Configurator is a separate PC application software, it is designed for hardware engineers to resolve pin conflicts for the RZA1 and RZG1 processors. The RZA1 SC does not support code generation. ( NOTE : RZA2M Smart Configurator is a different configurator that is integrated with the e2studio IDE ). This Software design kit includes scripts that generated code based on the exported pin settings. 

## Getting Started

[Demos Getting Started Guide](.\docs\RZA Microsoft Azure Getting Started Guide.md)
