######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 28 11:28:53 2017
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . \
              APP \
              CM3 \
              LCD5110 \
              MC20 \
              OBJ \
              APP\RTE \
              HARDWARE\KEY \
              HARDWARE\LED \
              HARDWARE\OLED \
              HARDWARE\USART2 \
              HARDWARE\USART3 \
              STM32F10x_FWLib\inc \
              STM32F10x_FWLib\src \
              SYSTEM\delay \
              SYSTEM\sys \
              SYSTEM\usart
INCLUDEPATH += .

# Input
HEADERS += APP/mb_gbk2uni.h \
           APP/stm32f10x.h \
           APP/stm32f10x_conf.h \
           APP/stm32f10x_it.h \
           APP/system_stm32f10x.h \
           CM3/core_cm3.h \
           LCD5110/LCD5110.h \
           LCD5110/Pixel_EN.h \
           MC20/mc20.h \
           OBJ/core_cm3.h \
           APP/RTE/RTE_Components.h \
           HARDWARE/KEY/key.h \
           HARDWARE/LED/led.h \
           HARDWARE/OLED/oled.h \
           HARDWARE/OLED/oledbmp.h \
           HARDWARE/OLED/oledfont.h \
           HARDWARE/USART2/usart2.h \
           HARDWARE/USART3/usart3.h \
           STM32F10x_FWLib/inc/misc.h \
           STM32F10x_FWLib/inc/stm32f10x_adc.h \
           STM32F10x_FWLib/inc/stm32f10x_bkp.h \
           STM32F10x_FWLib/inc/stm32f10x_can.h \
           STM32F10x_FWLib/inc/stm32f10x_cec.h \
           STM32F10x_FWLib/inc/stm32f10x_crc.h \
           STM32F10x_FWLib/inc/stm32f10x_dac.h \
           STM32F10x_FWLib/inc/stm32f10x_dbgmcu.h \
           STM32F10x_FWLib/inc/stm32f10x_dma.h \
           STM32F10x_FWLib/inc/stm32f10x_exti.h \
           STM32F10x_FWLib/inc/stm32f10x_flash.h \
           STM32F10x_FWLib/inc/stm32f10x_fsmc.h \
           STM32F10x_FWLib/inc/stm32f10x_gpio.h \
           STM32F10x_FWLib/inc/stm32f10x_i2c.h \
           STM32F10x_FWLib/inc/stm32f10x_iwdg.h \
           STM32F10x_FWLib/inc/stm32f10x_pwr.h \
           STM32F10x_FWLib/inc/stm32f10x_rcc.h \
           STM32F10x_FWLib/inc/stm32f10x_rtc.h \
           STM32F10x_FWLib/inc/stm32f10x_sdio.h \
           STM32F10x_FWLib/inc/stm32f10x_spi.h \
           STM32F10x_FWLib/inc/stm32f10x_tim.h \
           STM32F10x_FWLib/inc/stm32f10x_usart.h \
           STM32F10x_FWLib/inc/stm32f10x_wwdg.h \
           SYSTEM/delay/delay.h \
           SYSTEM/sys/core_cm3.h \
           SYSTEM/sys/sys.h \
           SYSTEM/usart/usart.h
SOURCES += APP/main.c \
           APP/mb_gbk2uni.c \
           APP/stm32f10x_it.c \
           APP/system_stm32f10x.c \
           CM3/core_cm3.c \
           LCD5110/LCD5110.c \
           MC20/mc20.c \
           OBJ/core_cm3.c \
           HARDWARE/KEY/key.c \
           HARDWARE/LED/led.c \
           HARDWARE/OLED/oled.c \
           HARDWARE/USART2/usart2.c \
           HARDWARE/USART3/usart3.c \
           STM32F10x_FWLib/src/misc.c \
           STM32F10x_FWLib/src/stm32f10x_adc.c \
           STM32F10x_FWLib/src/stm32f10x_bkp.c \
           STM32F10x_FWLib/src/stm32f10x_can.c \
           STM32F10x_FWLib/src/stm32f10x_cec.c \
           STM32F10x_FWLib/src/stm32f10x_crc.c \
           STM32F10x_FWLib/src/stm32f10x_dac.c \
           STM32F10x_FWLib/src/stm32f10x_dbgmcu.c \
           STM32F10x_FWLib/src/stm32f10x_dma.c \
           STM32F10x_FWLib/src/stm32f10x_exti.c \
           STM32F10x_FWLib/src/stm32f10x_flash.c \
           STM32F10x_FWLib/src/stm32f10x_fsmc.c \
           STM32F10x_FWLib/src/stm32f10x_gpio.c \
           STM32F10x_FWLib/src/stm32f10x_i2c.c \
           STM32F10x_FWLib/src/stm32f10x_iwdg.c \
           STM32F10x_FWLib/src/stm32f10x_pwr.c \
           STM32F10x_FWLib/src/stm32f10x_rcc.c \
           STM32F10x_FWLib/src/stm32f10x_rtc.c \
           STM32F10x_FWLib/src/stm32f10x_sdio.c \
           STM32F10x_FWLib/src/stm32f10x_spi.c \
           STM32F10x_FWLib/src/stm32f10x_tim.c \
           STM32F10x_FWLib/src/stm32f10x_usart.c \
           STM32F10x_FWLib/src/stm32f10x_wwdg.c \
           SYSTEM/delay/delay.c \
           SYSTEM/sys/sys.c \
           SYSTEM/usart/usart.c
