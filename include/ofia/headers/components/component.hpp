#pragma once

namespace ofia {
    class GameObject;  // forward declaration

    class Component
    {
    public:
        GameObject* gameObject = nullptr;
        virtual ~Component() = default;

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update(float dt) {}
        virtual void FixedUpdate(float dt) {}
        virtual void LateUpdate(float dt) {}
    };
}
