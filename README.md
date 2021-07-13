# RZA1 Microsoft Azure SDK

This is the RZA1 Software Development with Microsoft Azure Support. The SDK contains the RZA1 Board Support Package and drivers for the RZA1 LU, LC, L, M, and H target Processors, the Asure RTOS ( Threadx), Azure middleware ( USB Host, USB Device, Network, IoT, Filesystem, GUI Development). In driver GPIO code generation has been integrated with the RZA1 Smart Configurator.



## Microsoft Azure

The [Microsoft Azure software](https://azure.microsoft.com/en-us/free/search/?&ef_id=EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE:G:s&OCID=AID2100131_SEM_EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE:G:s&gclid=EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE) is supported by the Azure Microsoft Team.  This repository includes the middleware debug binaries. The full source code for the listed middleware can be found [here](https://github.com/azure-rtos/samples).

License

- **Threadx** 

  This is the Microsoft Azure RTOS

- **USBx**

  *Coming Soon*

  This is the USB middleware. It support both Host and Device. USB Class support include HID, MSC, CDC,   HUB, UVC, Audio, and printer.

- **GUIx**

  This the GUI middleware. It supports 16, 24 and 32 bit color space. Resolution is dependent on the amount of internal memory ( i.e. 2MB RZA1LC - 10MB RZA1H ) or external memory. 

- **FileX**

  This is the RTOS file system. Supports RAM disk, USB and SD ( requires request form Renesas ).

- **NetXDuo**

  This middware supplies the network for IP4, Ip6, TCP, UDP, HTTP, Secure Socket Layer, Crypto libraries, and many more.

- **Azure IoT**

  As part of the NetxDuo the Azure IoT Cloud connectivity is included. 



## RZA1 Smart Configuration Code Generation

The RZA1 Smart Configurator is a separate PC application software, it is designed for hardware engineers to resolve pin conflicts for the RZA1 and RZG1 processors. The RZA1 SC does not support code generation. ( NOTE : RZA2M Smart Configurator is a different configurator that is integrated with the e2studio IDE ). This Software design kit includes scripts that generated code based on the exported pin settings. 

![image-20210503181046632](C:\workspace\RZA1\Azure\rza1_gcc_azure_rtos_bsp_private\README.assets\image-20210503181046632.png)
