CFLAGS=-O3 -march=native -mtune=native -Wall -Wextra -pedantic -Ithird_party
CFLAGS_LIBPNG=`pkg-config --cflags libpng`
LDFLAGS_LIBPNG=`pkg-config --libs libpng`
INSTALL=`which install`
RM=`which rm`

qoibench:
	$(CC) $(CFLAGS) $(CFLAGS_LIBPNG) $(LDFLAGS_LIBPNG) -o qoibench qoibench.c

qoiconv:
	$(CC) $(CFLAGS) $(CFLAGS_LIBPNG) $(LDFLAGS_LIBPNG) -o qoiconv qoiconv.c

.PHONY: tools clean tools-install hdr-install\
	tools-uninstall hdr-uninstall\
	install uninstall

tools: qoibench qoiconv
tools-install: tools
	$(INSTALL) -o root -g wheel -m 755 qoibench ${prefix}/bin/qoibench
	$(INSTALL) -o root -g wheel -m 755 qoiconv ${prefix}/bin/qoiconv

tools-uninstall:
	$(RM) ${prefix}/bin/qoibench
	$(RM) ${prefix}/bin/qoiconv

hdr-install: qoi.h
	$(INSTALL) -o root -g wheel -m 755 qoi.h ${prefix}/include/qoi.h

hdr-uninstall:
	$(RM) ${prefix}/include/qoi.h

install: tools-install hdr-install
uninstall: tools-uninstall hdr-uninstall

clean:
	rm -rf *.o qoiconv qoibench
