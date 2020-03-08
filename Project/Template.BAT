SET PATH=D:\keil MDK\ARM\ARMCC\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SDKs\TypeScript\1.0\;C:\Program Files\Microsoft SQL Server\120\Tools\Binn\;D:\matlab2014a\runtime\win64;D:\matlab2014a\bin;D:\matlab2014a\polyspace\bin
SET CPU_TYPE=STM32F103ZE
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Template
SET CPU_CLOCK=0x007A1200
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\rcc.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\usart.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\misc.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_dma.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_sdio.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_usart.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_flash.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\sdio_sd.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\diskio.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\ff.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\ffsystem.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\ffunicode.__i"
"D:\keil MDK\ARM\ARMCC\Bin\ArmLink" --Via ".\Template.lnp"
"D:\keil MDK\ARM\ARMCC\bin\fromelf.exe" --bin -o  ../Objects/Template.bin  ../Project/@Template.axf
