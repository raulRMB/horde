#ifndef ATTRIBUTESYSTEM_H
#define ATTRIBUTESYSTEM_H

#include "systems/System.h"
#include "components/Attribute.h"

class SAttribute final : public UpdateSystem
{
public:
    void Update(float deltaSeconds) override;
    static void OnEffect(const FEffect& effect);
    void Init() override;
};


#endif
