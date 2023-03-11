CFLAGS := -O2 -Wall -Iinclude

SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)
OUT := lib6502.a

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
	@echo "  CC     $@"

$(OUT): $(OBJ)
	@ar rcs $@ $^
	@echo "  AR     $@"

.PHONY += clean
clean:
	rm -rf $(OBJ) $(OUT)
