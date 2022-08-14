## Sample usbx host mass storage

This sample is useing USBX Host MSC.
Please change the following definitions according to your environment.

    mcu_board_select.h
    Example:
    #define TARGET_RZA1  (TARGET_RZA1H)
    #define TARGET_BOARD (TARGET_BOARD_RSK)

Note<br>
RZ/A1H  Support DMA and PIO.<br>
RZ/A1LU Support PIO.<br>

If you define the following, you can enable DMA.<br>

    ux_hcd_rz.h
    #define UX_RZ_HCD_USE_DMA
