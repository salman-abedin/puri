PREFIX = /usr/local
install:
	@cp puri.sh puri
	@chmod 755 puri
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@mv puri ${DESTDIR}${PREFIX}/bin
	@echo Done installing executable files to ${DESTDIR}${PREFIX}/bin
uninstall:
	@rm -f ${DESTDIR}${PREFIX}/bin/puri
	@echo Done removing executable files from ${DESTDIR}${PREFIX}/bin
