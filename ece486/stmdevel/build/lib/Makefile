ROOTDIR = ..
include $(ROOTDIR)/Makefile.inc

ARCHMKFILES = libstm.mk \
              libcmsis_dsp.mk \
              libece486.mk

.PHONY : clean all install uninstall

all clean install uninstall:
	$(foreach mkfile,$(ARCHMKFILES), $(MAKE) ARCH=STM32L476xx -f $(mkfile) $@;)
	$(foreach mkfile,$(ARCHMKFILES), $(MAKE) ARCH=STM32L476xx -f $(mkfile) $@;)
	$(MAKE) -f linkerscript.mk $@
