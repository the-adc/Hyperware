#ifndef ENGINE_STATE_H_
#define ENGINE_STATE_H_

#include <nlohmann/json.hpp>

namespace engine {

using Json = nlohmann::ordered_json;

// An abstract class representing the state of an object.
//
// Author: Yash Mahmud
//
// Methods
// -------
// save(data):
//     Abstract method to save the state of an object.
// load(data):
//     Abstract method to load the state of an object.
class State {
public:
	virtual ~State() = default;

	// Author: Yash Mahmud
	//
	// Abstract method to save the state of an object.
	//
	// Parameters
	// ----------
	//     data : dict
	//         The dictionary to save the state of the object into.
	void Save(Json& /*data*/) {}

	// Author: Yash Mahmud
	//
	// Abstract method to load the state of an object.
	//
	// Parameters
	// ----------
	//     data : dict
	//         The dictionary to load the state of the object from.
	void Load(const Json& /*data*/) {}
};

}

#endif
