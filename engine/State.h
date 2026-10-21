#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#include <nlohmann/json.hpp>

using Json = nlohmann::ordered_json;

/*
An abstract class representing the state of an object.

Author: Yash Mahmud

Methods
-------
save(data):
    Abstract method to save the state of an object.
load(data):
    Abstract method to load the state of an object.
*/
class State {
public:
    virtual ~State() {}

    /*
    Author: Yash Mahmud

    Abstract method to save the state of an object.

    Parameters
    ----------
        data : dict
            The dictionary to save the state of the object into.
    */
    void save(Json &data) {}

    /*
    Author: Yash Mahmud

    Abstract method to load the state of an object.

    Parameters
    ----------
        data : dict
            The dictionary to load the state of the object from.
    */
    void load(const Json &data) {}
};

#endif
