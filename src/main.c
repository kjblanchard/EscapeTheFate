#include <Supergoon/engine.h>
#include <stdio.h>
static void Update(void) {
	puts("Hello world!");
}

int main(void) {
	SetUpdateFunction(&Update);
	Run();
}
