CC= gcc
CFLAGS= -lpq
DEPS= postgres.h
OBJ= test.o

%.o = %.c $(DEPS)
    $(CC) -c -o $@ $<
hellomake: $(OBJ)
  $(CC) -o $@ $^ $(CFLAGS)
