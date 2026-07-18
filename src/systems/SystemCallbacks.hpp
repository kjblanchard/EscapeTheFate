#pragma once
namespace Etf {
struct SystemCallbacks {
	void (*Start)() = nullptr;
	void (*Update)() = nullptr;
	void (*Draw)() = nullptr;
	void (*Shutdown)() = nullptr;
};
}  // namespace Etf
