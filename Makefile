#-- LICENSE: The MIT License(MIT) 
# Copyright(c) 2021 Dr Ashton Fagg <ashton@fagg.id.au>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files(the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and / or sell copies of the
# Software, and to permit persons to whom the Software is furnished to
# do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.  THE
# SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
