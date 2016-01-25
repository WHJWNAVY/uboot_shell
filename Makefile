CC	:= gcc
TARGET	:= main
SRC	:= *.c

all:$(TARGET)

$(TARGET):$(SRC)
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)

.PHONY:all clean
