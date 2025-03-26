#include <SDL3/SDL_main.h>
#include <Supergoon/engine.h>
#include <Supergoon/window.h>
#include <stdio.h>
static void Update(void) {
	(void)"Hello";
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	// SetWindowOptions(480, 270, "Escape The Fate");
	SetWindowOptions(1920, 1080, "Escape The Fate");
	SetScalingOptions(480, 270);
	SetUpdateFunction(&Update);
	Run();
}
