CC = g++
CFLAGS = -g -fdiagnostics-color=always -Iinclude
CLINK = -lWs2_32
TARGET = thttp

thttp:
	$(CC) $(CFLAGS) $(TARGET).cpp -o $(TARGET).exe $(CLINK)