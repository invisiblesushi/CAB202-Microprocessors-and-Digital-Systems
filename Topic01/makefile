TARGETS=hello hello_zdk banner

all: $(TARGETS)

LIBS=-lzdk -lncurses -lm
DIRS=-I../ZDK -L../ZDK
FLAGS=-std=gnu99 -Wall -Werror -g

clean:
	for f in $(TARGETS); do \
		if [ -f $$f ]; then rm $$f; fi; \
		if [ -f $$f.exe ]; then rm $$f.exe; fi; \
	done

rebuild: clean all

banner: banner.c
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

hello: hello.c
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 

hello_zdk: hello_zdk.c
	gcc $(FLAGS) $@.c -o $@ $(DIRS) $(LIBS) 
