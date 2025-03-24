#include <Supergoon/engine.h>
#include <Supergoon/window.h>
#include <stdio.h>
static void Update(void) {
	void;
}

int main(void) {
	// SetWindowOptions(480, 270, "Escape The Fate");
	SetWindowOptions(1920, 1080, "Escape The Fate");
	SetScalingOptions(480, 270);
	SetUpdateFunction(&Update);
	Run();
}
