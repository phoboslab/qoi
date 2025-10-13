CC ?= gcc
CFLAGS_BENCH ?= -std=gnu99 -O3
LFLAGS_BENCH ?= -lpng $(LDFLAGS)
CFLAGS_CONV ?= -std=c99 -O3
LFLAGS_CONV ?= $(LDFLAGS)

TARGET_BENCH ?= qoibench
TARGET_CONV ?= qoiconv

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
INCLUDEDIR ?= $(PREFIX)/include

all: $(TARGET_BENCH) $(TARGET_CONV)

bench: $(TARGET_BENCH)
$(TARGET_BENCH):$(TARGET_BENCH).c qoi.h
	$(CC) $(CFLAGS_BENCH) $(CFLAGS) $(TARGET_BENCH).c -o $(TARGET_BENCH) $(LFLAGS_BENCH)

conv: $(TARGET_CONV)
$(TARGET_CONV):$(TARGET_CONV).c qoi.h
	$(CC) $(CFLAGS_CONV) $(CFLAGS) $(TARGET_CONV).c -o $(TARGET_CONV) $(LFLAGS_CONV)

.PHONY: install
install: install-tools install-header

.PHONY: install-tools
install-tools: all
	install -Dm 755 $(TARGET_CONV) $(DESTDIR)$(BINDIR)/$(TARGET_CONV)
	install -Dm 755 $(TARGET_BENCH) $(DESTDIR)$(BINDIR)/$(TARGET_BENCH)

.PHONY: install-header
install-header: qoi.h
	install -Dm 644 qoi.h $(DESTDIR)$(INCLUDEDIR)/qoi.h

.PHONY: clean
clean:
	$(RM) $(TARGET_BENCH) $(TARGET_CONV)
