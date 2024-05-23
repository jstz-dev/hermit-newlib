AUTOMAKE_OPTIONS = cygnus
INCLUDES = $(NEWLIB_CFLAGS) $(CROSS_CFLAGS) $(TARGET_CFLAGS)
AM_CCASFLAGS = $(INCLUDES)
 
noinst_LIBRARIES = lib.a

lib_a_SOURCES = chown.c clock_getres.c clock_gettime.c clock_nanosleep.c clock_settime.c close.c environ.c errno.c execve.c _exit.c fork.c fstat.c getpagesize.c getpid.c gettod.c isatty.c kill.c link.c lseek.c nanosleep.c open.c read.c readlink.c sbrk.c sched.c setitimer.c signal.c stat.c symlink.c sysconf.c times.c unlink.c wait.c write.c context.S makecontext.c inet_ntoa.c
lib_a_CCASFLAGS = $(AM_CCASFLAGS)
lib_a_CFLAGS = $(AM_CFLAGS)
 
all-local: crt0.o
 
ACLOCAL_AMFLAGS = -I ../../..
CONFIG_STATUS_DEPENDENCIES = $(newlib_basedir)/configure.host

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/include/netinet; \
        for i in $(srcdir)/include/netinet/*.h; do \
          $(INSTALL_DATA) $$i $(DESTDIR)$(tooldir)/include/netinet/`basename $$i`; \
        done;
