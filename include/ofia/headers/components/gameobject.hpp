#pragma once
#include <memory>
#include <vector>
#include <string>

#include "component.hpp"
#include "transform.hpp"

namespace ofia
{
    class GameObject : public Component, public std::enable_shared_from_this<GameObject>
    {
    public:
        GameObject(const std::string& name)
            : name(name)
        {
            transform = std::make_unique<Transform>();
            transform->gameobject = this;
        }

        std::string name;
        std::unique_ptr<Transform> transform;

        std::vector<std::shared_ptr<Component>> components;

        std::weak_ptr<GameObject> parent;
        std::vector<std::shared_ptr<GameObject>> children;

        void AddChild(const std::shared_ptr<GameObject>& child)
        {
            child->parent = shared_from_this();
            children.push_back(child);
        }

        template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<Component, T>::value,
                          "T must be derived from Component");

            auto comp = std::make_shared<T>(std::forward<Args>(args)...);
            comp->gameObject = this;
            components.push_back(std::static_pointer_cast<Component>(comp));
            return comp;
        }

        template <typename T>
        std::shared_ptr<T> GetComponent()
        {
            for (auto& comp : components)
            {
                auto casted = std::dynamic_pointer_cast<T>(comp);
                if (casted)
                    return casted;
            }
            return nullptr;
        }

        template <typename T>
        std::vector<std::shared_ptr<T>> GetComponents()
        {
            std::vector<std::shared_ptr<T>> result;
            for (auto& comp : components)
            {
                auto casted = std::dynamic_pointer_cast<T>(comp);
                if (casted)
                    result.push_back(casted);
            }
            return result;
        }

        template <typename T>
        std::vector<std::shared_ptr<T>> GetComponentsRecursive()
        {
            std::vector<std::shared_ptr<T>> result;
            // Add components from this GameObject
            for (auto& comp : components)
            {
                auto casted = std::dynamic_pointer_cast<T>(comp);
                if (casted)
                    result.push_back(casted);
            }
            // Recursively get components from children
            for (const auto& child : children)
            {
                std::vector<std::shared_ptr<T>> childComponents = child->GetComponentsRecursive<T>();
                result.insert(result.end(), childComponents.begin(), childComponents.end());
            }
            return result;
        }
    };
}
