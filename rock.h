#ifndef SPIT_H
#define SPIT_H

#include <Urho3D/Urho3D.h>
#include "sceneobject.h"

class Rock : public SceneObject
{
    URHO3D_OBJECT(Rock, SceneObject);
public:
    Rock(Context* context);
    static void RegisterObject(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
    virtual void Set(Vector3 position);
private:
    Quaternion spin_;
};

#endif // SPIT_H
