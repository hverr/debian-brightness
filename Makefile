CC=gcc
CFLAGS=
LDFLAGS=

ODIR=build

TARGET=brightness

SOURCES=brightness.c
OBJECTS=$(SOURCES:%.c=$(ODIR)/%.o)

default: all

# Building

%.c : %.h

$(ODIR):
	mkdir $(ODIR)

$(ODIR)/%.o: %.c $(ODIR)
	$(CC) -c -o $@ $< $(CLFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Phonies
.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(ODIR)
	rm $(TARGET)

.PHONY: install
install: all
	install $(TARGET) $(PREFIX)/bin/

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin

