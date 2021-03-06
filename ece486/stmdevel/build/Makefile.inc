
STCUBEBASE=$(ROOTDIR)/STM32cubeL4/STM32Cube_FW_L4_V1.10.0/
# CMSISBASE=$(ROOTDIR)/cmsis/CMSIS/
CMSISBASE=$(STCUBEBASE)/Drivers/CMSIS/

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy
RANLIB=arm-none-eabi-ranlib

PRJCFLAGS = -O3 -mcpu=cortex-m4 -mthumb -fomit-frame-pointer -fno-strict-aliasing -fdata-sections -include stm32l4xx_hal_conf.h -DARM_MATH_CM4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -fsingle-precision-constant -ffunction-sections

LIBDIR = $(ROOTDIR)/lib
INCLUDEDIR = $(ROOTDIR)/include

INSTALLDIR = /usr/local/stmdev
INSTALLLIB = $(INSTALLDIR)/lib
INSTALLINC = $(INSTALLDIR)/include
