src = $(wildcard AIEngine/*.c) $(wildcard IOinterface/*.c) $(wildcard CLI/*.c) $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))
target = a
CC = gcc

$(target): $(obj)
	$(CC) $(obj) -o $(target) -lpthread

%.o: %.c
	$(CC) -O3 -c $< -o $@

.PHONY: clean all
clean:
	rm -f $(obj)
all:
	@echo $(src)
	@echo $(obj)
