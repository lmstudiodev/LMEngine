#pragma once
#include <Prerequisites.h>

class Entity
{
public:
	Entity();
	virtual ~Entity();

public:
	void release();

	World* getWorld();
	InputSystem* getInputSystem();
	TransformComponent* getTransformComponent();

	template <typename T>
	T* createComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component class.");
		
		auto c = getComponent<T>();

		if (!c)
		{
			auto id = typeid(T).hash_code();
			c = new T();
			createComponentInternal(c, id);

			return c;
		}

		return nullptr;
	}

	template <typename T>
	T* getComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component class.");

		auto id = typeid(T).hash_code();

		return (T*)getComponentInternal(id);
	}

private:
	void createComponentInternal(Component* component, size_t id);
	Component* getComponentInternal(size_t id);
	void removeComponent(size_t id);

protected:
	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}

protected:
	size_t m_type_id = 0;
	World* m_world = nullptr;

	TransformComponent* m_transformComponent = nullptr;
	std::map <size_t, std::unique_ptr<Component>> m_components;

private:
	friend class World;
	friend class Component;
};

