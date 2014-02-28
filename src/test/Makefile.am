TESTS          = testCmdLine
check_PROGRAMS = $(TESTS)
bin_PROGRAMS   = test_CmdLine 
AM_CPPFLAGS    = -I$(top_srcdir)/src/libcmdline

test_CmdLine_SOURCES = test_CmdLine.cc
test_CmdLine_LDADD = $(top_builddir)/src/libcmdline/.libs/libcmdline.a

testCmdLine_SOURCES = testCmdLine.cc
testCmdLine_LDADD = $(top_builddir)/src/libcmdline/.libs/libcmdline.a

EXTRA_DIST = lmbunit.hh
