
ROOTDIR = ..
include $(ROOTDIR)/Makefile.inc

INCDIRS = ${STCUBEBASE}/Drivers/STM32L4xx_HAL_Driver/Inc \
          ${STCUBEBASE}/Drivers/CMSIS/Device/ST/STM32L4xx/Include \
          ${STCUBEBASE}/Drivers/CMSIS/Include \
          ${STCUBEBASE}/Drivers/BSP/STM32L476G-Discovery \
          ${STCUBEBASE}/Drivers/BSP/Components/Common \
          ${STCUBEBASE}/Utilities/Fonts/ \
          $(ROOTDIR)/src/stm32l476g-discovery-bsp-modified/ \
          $(ROOTDIR)/src/libece486/

.PHONY : clean all install uninstall

all:
	$(foreach incdir,$(INCDIRS), cp $(incdir)/*.h .;)
	cp -r $(STCUBEBASE)/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy .
	cp $(STCUBEBASE)/Projects/STM32L476G-Discovery/Templates/Inc/stm32l4xx_hal_conf.h .
	cp $(CMSISBASE)/Include/arm_math.h .
	cp $(CMSISBASE)/Include/arm_common_tables.h .


install: all
	mkdir -p $(INSTALLINC)
	cp *.h $(INSTALLINC)
	cp -r Legacy $(INSTALLINC)

uninstall:
	rm $(INSTALLINC)/*.h
	rm -rf $(INSTALLINC)/Legacy

clean:
	rm -f *.h
	rm -rf Legacy
