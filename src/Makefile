CC=gcc
CFLAGS=-Wall -Werror -Wextra
all : main run

main : 
	$(CC) $(CFLAGS) main.c -o to_do_list

clean:
	rm -rf ../build/to_do_list ./a.out to_do_list

run: 
	./to_do_list
	
clang:
	clang-format -i *.c *.h 