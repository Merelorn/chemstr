CC = g++
CFLAGS = -std=c++11 -c 
LFLAGS = -L/home/wondrash/c++/lib 
LIBS = -Wl,-Bstatic -lprops -lstdc++ -Wl,-Bdynamic
INCLUDES = -I/home/wondrash/c++/include/eigen -I/home/wondrash/c++/include 
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

default: strpams ghash mirrorflip dihedral_list dihedral_compare idecon unimorph
	@echo "chemstr has been compiled"
	@cp ./{ghash,strpams,mirrorflip,dihedral_list,dihedral_compare,angle_compare} ~/bin/

libchemstr: $(OBJS)
	$(CC) -shared chemstr.o -o /home/wondrash/c++/lib/libchemstr.so

idecon: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) chemstr.o idecon.o -o idecon $(LIBS)

unimorph: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) chemstr.o unimorph.o fg.o hung.o -o unimorph $(LIBS)

dihedral_compare: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) chemstr.o dihedral_compare.o $(LIBS) -o dihedral_compare

dihedral_list: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) chemstr.o dihedral_list.o $(LIBS) -o dihedral_list

strpams: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) chemstr.o strpams.o $(LIBS) -o strpams

ghash: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) chemstr.o ghash.o -o ghash $(LIBS) 

mirrorflip: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) chemstr.o mirrorflip.o $(LIBS) -o mirrorflip 

%.o:%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm $(OBJS) strpams ghash mirrorflip dihedral_list dihedral_compare idecon unimorph
