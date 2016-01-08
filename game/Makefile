OBJS1	=	server_main.o 
OBJS2	=	client_main.o client_net.o client_command.o client_win2.o client_system.o
TARGET1	=	server
TARGET2	=	client
CFLAGS	=	-c -DNDEBUG
WII     =       -lcwiimote -D_ENABLE_TILT -D_ENABLE_FORCE -L/usr/lib

.c.o:
	gcc $(CFLAGS) $<

all: $(TARGET1) $(TARGET2)

$(TARGET1):	$(OBJS1)
	gcc -o $(TARGET1) $(OBJS1) -lm -lSDL $(WII)
$(TARGET2):	$(OBJS2)
	gcc -o $(TARGET2) $(OBJS2) -lm -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf $(WII) $(LDFLAGS)

clean:
	rm *.o $(TARGET1) $(TARGET2)

server_main.o: server_main.c  common.h
client_main.o: client_main.c client_func.h common.h
client_net.o: client_net.c client_func.h common.h
client_command.o: client_command.c client_func.h common.h
client_win2.o: client_win2.c client_func.h common.h
client_system.o: client_system.c client_func.h common.h
