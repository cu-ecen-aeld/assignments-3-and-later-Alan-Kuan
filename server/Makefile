CC=${CROSS_COMPILE}gcc
EXE=aesdsocket

.PHONY: all default

all: $(EXE)
default: $(EXE)

$(EXE): $(EXE).c
	$(CC) -o $@ $<