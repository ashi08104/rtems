
rtems_tests_PROGRAMS = spfreechain03
spfreechain03_SOURCES = init.c

dist_rtems_tests_DATA = spfreechain03.scn
dist_rtems_tests_DATA += spfreechain03.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(spfreechain03_OBJECTS)
LINK_LIBS = $(spfreechain03_LDLIBS)

spfreechain03$(EXEEXT): $(spfreechain03_OBJECTS) $(spfreechain03_DEPENDENCIES)
	@rm -f spfreechain03$(EXEEXT)
	$(make-exe)
