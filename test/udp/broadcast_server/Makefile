TARGET = server
FLAG = -O2 -Wall
INC_PATH =
LIB_PATH =
LIBS = -lrt -lpthread
all:
	gcc $(FLAG) $(INC_PATH) broadcast_server.c -o $(TARGET) $(LIB_PATH) $(LIBS)

clean:
	rm $(TARGET)
