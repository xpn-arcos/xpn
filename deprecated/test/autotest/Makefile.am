EXTRA_DIST = testsuite.at $(TESTSUITE)
TESTSUITE = $(srcdir)/testsuite

check-local: atconfig $(TESTSUITE)
	mkdir -p $(abs_srcdir)/prueba
	chmod 777 $(abs_srcdir)/prueba
	${top_srcdir}/fuse/fuse-expand $(abs_srcdir)/prueba
#	$(SHELL) '$(TESTSUITE)' $(TESTSUITEFLAGS)
	TDIRECTORY=$(abs_srcdir)/prueba $(TESTSUITE) || true
	fusermount -u $(abs_srcdir)/prueba
	rm -rf $(abs_srcdir)/prueba

installcheck-local: atconfig $(TESTSUITE)
	$(SHELL) '$(TESTSUITE)' AUTOTEST_PATH='$(bindir)' \
	$(TESTSUITEFLAGS)

clean-local:
	test ! -f '$(TESTSUITE)' || \
	$(SHELL) '$(TESTSUITE)' --clean

ltp-full-@LTP_VERSION@/lib/Makefile:
#	cd ${top_srcdir}/test/POSIX/
	tar zxvf ${top_srcdir}/test/POSIX/ltp-full-@LTP_VERSION@.tgz

ltp-full-@LTP_VERSION@/testcases/kernel/syscalls/writev/writev06: ltp-full-@LTP_VERSION@/lib/Makefile
	make -C ltp-full-@LTP_VERSION@

AUTOTEST = $(AUTOM4TE) --language=autotest
$(TESTSUITE): $(srcdir)/package.m4 $(srcdir)/testsuite.at ltp-full-@LTP_VERSION@/testcases/kernel/syscalls/writev/writev06
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $@.at
	mv $@.tmp $@

# The `:;' works around a Bash 3.2 bug when the output is not writeable.
$(srcdir)/package.m4: $(top_srcdir)/configure.ac
	:;{ \
		echo '# Signature of the current package.' && \
		echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])' && \
		echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])' && \
		echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])' && \
		echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])' && \
		echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
	} >'$(srcdir)/package.m4'
