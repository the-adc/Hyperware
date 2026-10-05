#include "engine/app.h"

#include <SFML/Window/Event.hpp>

#include <filesystem>
#include <optional>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "engine/assets.h"
#include "engine/audio.h"
#include "engine/input.h"

namespace engine {
namespace {

namespace fs = std::filesystem;

// the folder the exe is in, empty if we can't tell
fs::path ExeFolder() {
#ifdef _WIN32
	wchar_t path[MAX_PATH];
	DWORD length = GetModuleFileNameW(nullptr, path, MAX_PATH);
	if (length > 0 && length < MAX_PATH) {
		return fs::path(path).parent_path();
	}
#else
	std::error_code error;
	fs::path exe = fs::read_symlink("/proc/self/exe", error);
	if (!error) {
		return exe.parent_path();
	}
#endif
	return fs::path();
}

std::string FindAssetsRoot() {
	std::error_code error;
	if (fs::exists("assets", error)) {
		return "assets";
	}
	fs::path beside_exe = ExeFolder() / "assets";
	if (fs::exists(beside_exe, error)) {
		return beside_exe.string();
	}
	return "assets";
}

}  // namespace

class AppWorld : public World {
public:
	explicit AppWorld(App* app) : World(&app->display_), app_(app) {}

	bool StillRunning() const override { return app_->running_; }

protected:
	void BeginFrame() override {
		app_->PumpEvents();
		input::BeginStep(app_->display_);
	}

private:
	App* app_;
};

void App::PumpEvents() {
	while (const std::optional event = display_.window().pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			Quit();
		} else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
			// Alt+Enter, like the Windows game options in GameMaker
			if (key->code == sf::Keyboard::Key::Enter && key->alt) {
				display_.ToggleFullscreen();
			}
		}
	}
}

bool App::Init(const AppConfig& config) {
	if (!display_.Init(config.title, config.start_fullscreen)) {
		return false;
	}
	display_.SetCursorVisible(!config.hide_cursor);

	audio::Init();

	assets_root_ = FindAssetsRoot();
	if (!assets::Load(assets_root_)) {
		return false;
	}

	world_ = std::make_unique<AppWorld>(this);
	running_ = true;
	return true;
}

void App::Shutdown() {
	if (world_) {
		world_->Clear();
	}
	assets::Unload();
	audio::Shutdown();
	display_.Shutdown();
	world_.reset();
}

void App::Run() { world_->Run(); }

}  // namespace engine
