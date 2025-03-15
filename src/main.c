#include <Supergoon/engine.h>
#include <Supergoon/window.h>
#include <stdio.h>
static void Update(void) {
	puts("Hello world!");
}

int main(void) {
	SetWindowOptions(640, 480, "Escape The Fate");
	SetUpdateFunction(&Update);
	Run();
}
