
#
# TBD
#

lib_LTLIBRARIES = libdrmaav2.la

libdrmaav2includedir = /drmaav2/c-binding/

libdrmaav2_la_SOURCES = c-binding/drmaa2.cpp
  
libdrmaav2include_HEADERS = c-binding/drmaa2.h

libdrmaav2_la_LIBADD = $(top_srcdir)/src/libsrc.la

libdrmaav2_la_CPPFLAGS = -I$(top_srcdir)/api/c-binding -I$(top_srcdir)/api/cpp-binding

libdrmaav2_la_LDFLAGS = $(COVERAGE_LDFLAGS)

clean-local:
	rm -rf *.info *.gcno *.png *.html *.o *.lo

