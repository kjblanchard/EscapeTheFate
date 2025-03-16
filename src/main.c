#include <Supergoon/engine.h>
#include <Supergoon/window.h>
#include <stdio.h>
static void Update(void) {
	puts("Hello world!");
}

int main(void) {
	SetWindowOptions(1024, 576, "Escape The Fate");
	SetScalingOptions(512, 288);
	SetUpdateFunction(&Update);
	Run();
}
