
rtems_tests_PROGRAMS = spfreechain02
spfreechain02_SOURCES = init.c

dist_rtems_tests_DATA = spfreechain02.scn
dist_rtems_tests_DATA += spfreechain02.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(spfreechain02_OBJECTS)
LINK_LIBS = $(spfreechain02_LDLIBS)

spfreechain02$(EXEEXT): $(spfreechain02_OBJECTS) $(spfreechain02_DEPENDENCIES)
	@rm -f spfreechain02$(EXEEXT)
	$(make-exe)
