#include <Supergoon/engine.h>
#include <Supergoon/window.h>
#include <stdio.h>
static void Update(void) {
	void;
}

int main(void) {
	// SetWindowOptions(1024, 576, "Escape The Fate");
	SetWindowOptions(512, 288, "Escape The Fate");
	SetScalingOptions(512, 288);
	SetUpdateFunction(&Update);
	Run();
}
