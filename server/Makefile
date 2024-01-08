CC=${CROSS_COMPILE}gcc
EXE=aesdsocket

.PHONY: all default clean

all: $(EXE)
default: $(EXE)

$(EXE): $(EXE).c
	$(CC) -o $@ $<

clean:
	-rm $(EXE)