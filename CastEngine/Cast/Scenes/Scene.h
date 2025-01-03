#pragma once

class Scene {

    public:
        virtual bool init() = 0;
        virtual void update(float delta) = 0;
        virtual void render(float delta) = 0;
        virtual bool cleanup() = 0;
        virtual ~Scene() = default;
};
