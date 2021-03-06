ROOTDIR = ..
include $(ROOTDIR)/Makefile.inc

# Default Architecture 
# (normally overridden by make command line: "make ARCH=STM32F407xx -f thisfile")
ARCH = STM32L476xx

TARGET=lib$(ARCH).a

DRIVEROBJS= stm32l4xx_hal_adc.o \
    stm32l4xx_hal_adc_ex.o \
    stm32l4xx_hal.o \
    stm32l4xx_hal_can.o \
    stm32l4xx_hal_comp.o \
    stm32l4xx_hal_cortex.o \
    stm32l4xx_hal_crc.o \
    stm32l4xx_hal_crc_ex.o \
    stm32l4xx_hal_cryp.o \
    stm32l4xx_hal_cryp_ex.o \
    stm32l4xx_hal_dac.o \
    stm32l4xx_hal_dac_ex.o \
    stm32l4xx_hal_dfsdm.o \
    stm32l4xx_hal_dma.o \
    stm32l4xx_hal_firewall.o \
    stm32l4xx_hal_flash.o \
    stm32l4xx_hal_flash_ex.o \
    stm32l4xx_hal_flash_ramfunc.o \
    stm32l4xx_hal_gpio.o \
    stm32l4xx_hal_hcd.o \
    stm32l4xx_hal_i2c.o \
    stm32l4xx_hal_i2c_ex.o \
    stm32l4xx_hal_irda.o \
    stm32l4xx_hal_iwdg.o \
    stm32l4xx_hal_lcd.o \
    stm32l4xx_hal_lptim.o \
    stm32l4xx_hal_msp_template.o \
    stm32l4xx_hal_nand.o \
    stm32l4xx_hal_nor.o \
    stm32l4xx_hal_opamp.o \
    stm32l4xx_hal_opamp_ex.o \
    stm32l4xx_hal_pcd.o \
    stm32l4xx_hal_pcd_ex.o \
    stm32l4xx_hal_pwr.o \
    stm32l4xx_hal_pwr_ex.o \
    stm32l4xx_hal_qspi.o \
    stm32l4xx_hal_rcc.o \
    stm32l4xx_hal_rcc_ex.o \
    stm32l4xx_hal_rng.o \
    stm32l4xx_hal_rtc.o \
    stm32l4xx_hal_rtc_ex.o \
    stm32l4xx_hal_sai.o \
    stm32l4xx_hal_sd.o \
    stm32l4xx_hal_smartcard.o \
    stm32l4xx_hal_smartcard_ex.o \
    stm32l4xx_hal_smbus.o \
    stm32l4xx_hal_spi.o \
    stm32l4xx_hal_spi_ex.o \
    stm32l4xx_hal_sram.o \
    stm32l4xx_hal_swpmi.o \
    stm32l4xx_hal_tim.o \
    stm32l4xx_hal_tim_ex.o \
    stm32l4xx_hal_tsc.o \
    stm32l4xx_hal_uart.o \
    stm32l4xx_hal_uart_ex.o \
    stm32l4xx_hal_usart.o \
    stm32l4xx_hal_wwdg.o \
    stm32l4xx_ll_fmc.o \
    stm32l4xx_ll_sdmmc.o \
    stm32l4xx_ll_usb.o 

VPATH = ${STCUBEBASE}/Drivers/STM32L4xx_HAL_Driver/Src/ \
        $(STCUBEBASE)/Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc/ \
        $(STCUBEBASE)/Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/ \
        $(ROOTDIR)/src/stm32l476g-discovery-bsp-modified/

ifeq ($(ARCH),STM32L476xx)
  BSPOBJS = startup_stm32l476xx.o \
            system_stm32l4xx.o \
            stm32l476g_discovery.o \
    cs43l22.o \
    l3gd20.o \
    mfxstm32l152.o \
    stm32l476g_discovery_audio.o \
    stm32l476g_discovery.o \
    stm32l476g_discovery_glass_lcd.o \
    stm32l476g_discovery_gyroscope.o \
    stm32l476g_discovery_idd.o \
    stm32l476g_discovery_qspi.o \
    lsm303c.o \
    stm32l476g_discovery_compass.o 
endif

OBJS = $(addsuffix .$(ARCH), $(DRIVEROBJS)) $(addsuffix .$(ARCH), $(BSPOBJS))

CFLAGS = $(PRJCFLAGS) -D$(ARCH) -I. -I$(INCLUDEDIR)

.PHONY : clean all depend install uninstall

all: $(TARGET)

%.o.$(ARCH) : %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.o.$(ARCH): %.s
	$(COMPILE.s) -o $@ $<

pdm_filter.o.$(ARCH): $(ROOTDIR)/src/stm_bsp_modified/libPDMFilter_CM4F_GCC.a
	$(AR) -xv $< pdm_filter.o
	mv pdm_filter.o pdm_filter.o.$(ARCH)

${TARGET}: ${OBJS}
	${AR} rc ${TARGET} ${OBJS}
	${RANLIB} ${TARGET}

install: $(TARGET)
	mkdir -p $(INSTALLLIB)
	cp $(TARGET) $(INSTALLLIB)

uninstall:
	rm $(INSTALLLIB)/$(TARGET)

clean:
	rm -f *.o ${TARGET}
