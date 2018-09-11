struct foo {
	char a : 1;
	char b : 1;
	int c;
};


#define mem_location 0x12345678

struct foo *p_foo = mem_location;


p_foo -> a = 1;


char *p = mem_location;

p[0] = 1;
p[0] = 2;

p[0] |= 4;
p[0] &= ~4;


p_foo -> a = 2;
