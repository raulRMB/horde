#ifndef ATTRIBUTESYSTEM_H
#define ATTRIBUTESYSTEM_H

#include "systems/System.h"
#include "components/Components.h"

class AttributeSystem final : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
    static void OnEffect(const Effect& effect);
    void Init() override;
};


#endif
