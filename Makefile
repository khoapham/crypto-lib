#!/usr/bin/make -f <

NAME	= libapm
#VERSION	= $(shell sh -c 'git describe --tags --dirty=-dev 2> /dev/null || echo 0')

SRC	= \
	e_apm.c \
	ciphers.c \
	digests.c

EXTRA_FILES	= \
	README

CFLAGS	= \
	-O3 \
	-W -Wall -Werror

SPEC_CFLAGS	= \
	-fPIC

LDFLAGS	=

SPEC_LDFLAGS	= \
	-lcrypto

CC	= $(CROSS_COMPILE)gcc
LD	= $(CC)

STRIP	= strip
STRIP_FLAGS	= --strip-unneeded -R .comment -R .GCC.command.line -R .note.gnu.build-id

RM	= rm -f

TAR	= tar
XZ	= xz

ifeq ($(V),)
 Q	= @

 qcmd	= @printf "  %-8s %s\n" "$(1)" "$(2)";
endif   # $(V)

all:	shared

shared:	$(NAME).so
strip:	$(NAME).so.strip
dist:	$(NAME)-$(VERSION).txz

%.o:    %.c
	$(call qcmd,CC,$@)
	$(Q)$(CC) -I. -c $(SPEC_CFLAGS) $(CFLAGS) -o $@ $<

$(NAME).so:	$(SRC:c=o)
	$(call qcmd,LD,$@)
	$(Q)$(LD) -shared -o $@ $^ $(SPEC_LDFLAGS) $(LDFLAGS) -L.

$(NAME).so.strip:       $(NAME).so
	$(call qcmd,STRIP,$@)
	$(Q)$(STRIP) $(STRIP_FLAGS) -o $@ $<

$(NAME)-$(VERSION).tar: $(SRC) $(EXTRA_FILES) Makefile
	$(call qcmd,TAR,$@)
	$(Q)$(TAR) cf $@ \
		--transform "s,^,$(NAME)-$(VERSION)/," \
		--force-local --numeric-owner \
		$^

%.txz:  %.tar
	$(call qcmd,XZ,$@)
	$(Q)$(XZ) -c > $@ < $<

clean:
	$(call qcmd,RM,$(SRC:c=o))
	$(Q)$(RM) $(SRC:c=o)
	$(call qcmd,RM,$(NAME).so)
	$(Q)$(RM) $(NAME).so $(NAME).so.strip

dist-clean: clean
	$(call qcmd,RM,$(NAME)-$(VERSION).tar)
	$(Q)$(RM) $(NAME)-$(VERSION).tar
	$(call qcmd,RM,$(NAME)-$(VERSION).txz)
	$(Q)$(RM) $(NAME)-$(VERSION).txz
