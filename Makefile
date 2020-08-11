include config.mk

OBJ = $(SRC:.c=.o)
SRC = $(wildcard *.c)

all: $(NAME)
$(NAME): $(OBJ)
install: all
	@mkdir -p "$(DESTDIR)$(BINPREFIX)"
	@mv $(NAME) "$(DESTDIR)$(BINPREFIX)"
	@rm -f $(OBJ)
	@echo Done moving the binary to ${DESTDIR}${BINPREFIX}
clean:
	@rm -f $(OBJ) $(NAME)
	@echo Done cleaning the source
uninstall: clean
	@rm -f "$(DESTDIR)$(BINPREFIX)/$(NAME)"
	@echo Done removing the binary from ${DESTDIR}${BINPREFIX}
.PHONY: all install uninstall clean
