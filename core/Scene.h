#ifndef SCENE_H
#define SCENE_H


class Scene
{
private:


public:
    virtual ~Scene() = default;
    Scene() = default;

    virtual void Start() = 0;
    virtual void HandleInput() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void DrawUI() = 0;
    virtual void Clean() = 0;
    virtual void Save() = 0;
    virtual void Load() = 0;
};


#endif //SCENE_H
