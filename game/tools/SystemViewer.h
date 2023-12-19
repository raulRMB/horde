#ifndef HORDE_SYSTEMVIEWER_H
#define HORDE_SYSTEMVIEWER_H

#include <entt/entt.hpp>

namespace tZ
{

class SystemViewer
{
public:
    SystemViewer();
    ~SystemViewer();

    static SystemViewer& Instance();

    void Init();
    void Update(float deltaSeconds);
    void Draw();

private:

};

}

#endif //HORDE_SYSTEMVIEWER_H
