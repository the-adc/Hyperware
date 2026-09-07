#ifndef HYPER_ENGINE_ACTORS_ATTRIBUTES_ACTOR_ATTRIBUTE_H_
#define HYPER_ENGINE_ACTORS_ATTRIBUTES_ACTOR_ATTRIBUTE_H_

namespace engine {

// Attributes are like variables, just with maximums, I think
class ActorAttribute {
public:
	virtual ~ActorAttribute() = default;

	virtual void Update(double points) = 0;
	virtual void Increase(double points) = 0;
	virtual void Decrease(double points) = 0;
	virtual double Get() const = 0;

	virtual double GetMaximum() const { return Get(); }
	virtual void UpdateMaximum(double points) { Update(points); }
	virtual void IncreaseMaximum(double points) { Increase(points); }
	virtual void DecreaseMaximum(double points) { Decrease(points); }
};

}  // namespace engine

#endif  // HYPER_ENGINE_ACTORS_ATTRIBUTES_ACTOR_ATTRIBUTE_H_
