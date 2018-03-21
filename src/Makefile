CC = gcc
BUILDDIR = build

PLGNSRC = openvpn-plugin-radccd.c radius.c ccdfile.c common.c
PLGNOBJ = $(patsubst %.c,$(BUILDDIR)/%.o,$(PLGNSRC))
PLGNSNM = openvpn-plugin-radccd
PLGNCFL = -O2 -fPIC -Wall -Wextra $(CFLAGS)

all: dir $(BUILDDIR)/$(PLGNSNM).so

$(BUILDDIR)/$(PLGNSNM).so: $(PLGNOBJ)
	$(CC) $(PLGNCFL) -shared $^ -o $@ -lradcli

$(PLGNOBJ): $(BUILDDIR)/%.o : %.c dir
	$(CC) $(PLGNCFL) -c $< -o $@

test: $(PLGNOBJ)
	$(CC) $(PLGNCFL) -o $(BUILDDIR)/test test.c $^ -lradcli

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean install uninstall
