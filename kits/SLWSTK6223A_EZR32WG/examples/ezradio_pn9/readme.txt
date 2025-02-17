EZRadio PN9 transmission example using the EZRadio device in the EZR32 device.

This example project uses the EZR32WG CMSIS including emlib, emdrv and the
board support package support functions/drivers to demonstrate driving
the EZRadio or EZRadioPRO device in the EZR32 device.

The user can start PN9 (pseudorandom binary where N=9) transmission by pressing
the PB0 button. Ceasing the transmission can be issued by pressing the PB0 
button again. 
See the EzRadio documentation for more information on PN9 transmission.

The sample app is able to consume radio configuration header files generated
with either Simplicity Studio or Wireless Development Suite. The generated
configuration is consumed automatically in Simplicity Studio. In order to use
the generated header file instead of the default one with other toolchains enable
RADIO_USE_GENERATED_CONFIGURATION in ezradio_plugin_manager.h.

For more information please refer to the Quick Start Guide of the application.

Board:  Silicon Labs SLWSTK6223A_EZR32WG Development Kit
Device: EZR32WG330F256R63
