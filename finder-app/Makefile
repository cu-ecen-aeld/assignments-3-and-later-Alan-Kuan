CC=$(CROSS_COMPILE)gcc
EXE=writer

.PHONY: all clean

all: $(EXE)

$(EXE): $(EXE).o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -o $@ -c $<

clean:
	-rm *.o