CC = g++
CFLAGS = -std=c++11 -c 
LFLAGS = -L/home/wondrash/c++/lib 
LIBS = -Wl,-Bstatic -lprops -lstdc++ -Wl,-Bdynamic
INCLUDES = -I/home/wondrash/c++/include/eigen -I/home/wondrash/c++/include 
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

default: ghash strpams mirrorflip dihedral_list dihedral_compare angle_compare angle_list 
	@echo "read_in has been compiled"
	@cp ./{ghash,strpams,mirrorflip,dihedral_list,dihedral_compare,angle_compare} ~/bin/

test: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o test.o recognize.o -o test $(LIBS)

idestr: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o idestr.o -o idestr

assign: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o assign.o recognize.o hung.o -o unimorph $(LIBS)

CDN: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o CDN_mutate.o recognize.o $(LIBS) -o CDN_mutate
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o CDN_id.o recognize.o $(LIBS) -o CDN_id
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o recognize.o CDN_id2str.o $(LIBS) -o CDN_id2str
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o idecon.o recognize.o $(LIBS) -o idecon 
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o CDN_2D.o recognize.o $(LIBS) -o CDN_2D 

angle_compare: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o angle_compare.o $(LIBS) -o angle_compare

angle_list: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o angle_list.o $(LIBS) -o angle_list

dihedral_compare: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o dihedral_compare.o $(LIBS) -o dihedral_compare

dihedral_list: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o dihedral_list.o $(LIBS) -o dihedral_list

strpams: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o strpams.o $(LIBS) -o strpams

ghash: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o ghash.o -o ghash $(LIBS) 

mirrorflip: $(OBJS)
	$(CC) $(INCLUDES) $(LFLAGS) read_in.o mirrorflip.o $(LIBS) -o mirrorflip 

%.o:%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm $(OBJS) strpams ghash mirrorflip dihedral_list
