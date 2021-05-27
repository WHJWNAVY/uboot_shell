CC	:= gcc
TARGET	:= main
SRC	:= *.c
src_dir=$(pwd)

all:$(TARGET)

$(TARGET):$(SRC)
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)

.PHONY:all clean

lint:
	find ${src_dir} -iname "*.[ch]" | xargs clang-format-6.0 -i
