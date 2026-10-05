#ifndef HYPER_ENGINE_APP_H_
#define HYPER_ENGINE_APP_H_

#include <memory>
#include <string>

#include "engine/display/display.h"
#include "engine/positions/world.h"

namespace engine {

struct AppConfig {
	std::string title = "Hyperware Game";
	bool start_fullscreen = false;
	bool hide_cursor = false;
};

class App {
public:
	App() = default;
	~App() = default;

	App(const App&) = delete;
	App& operator=(const App&) = delete;

	// false (and says why on stderr) if the window or the assets fail
	bool Init(const AppConfig& config);
	void Shutdown();

	void Run();
	void Quit() { running_ = false; }
	bool running() const { return running_; }

	World* world() const { return world_.get(); }
	Display* display() { return &display_; }
	// "assets" in the working directory if it's there, else next to the exe
	const std::string& assets_root() const { return assets_root_; }

private:
	friend class AppWorld;

	void PumpEvents();

	bool running_ = false;
	std::string assets_root_;
	Display display_;
	std::unique_ptr<World> world_;
};

}  // namespace engine

#endif  // HYPER_ENGINE_APP_H_
