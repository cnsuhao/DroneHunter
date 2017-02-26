/* heXon
// Copyright (C) 2017 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef SPAWNMASTER_H
#define SPAWNMASTER_H

#include <Urho3D/Urho3D.h>

#include "mastercontrol.h"

#define PREDATORSPAWNPOINT Vector3(LucKey::RandomSign() * 10.0f, 1.25f, 13.0f)
#define REAPERSPAWNPOINT Vector3(LucKey::RandomSign() * 5.0f, 1.75f, 13.0f)

#define TR3BINTERVAL Random(23.0f, 100.0f)
#define TR3BSPAWNPOINT Vector3(LucKey::RandomSign() * 23.0f, 2.3f, 0.0f)

class SpawnMaster : public Object
{
    friend class MasterControl;
    URHO3D_OBJECT(SpawnMaster, Object);
public:
    SpawnMaster(Context* context);

    void Clear();
    Vector3 SpawnPoint();

    template <class T> T* Create(bool recycle = true)
    {
        T* created{ nullptr };

        if (recycle) {
            PODVector<Node*> correctType{};
            MC->scene_->GetChildrenWithComponent<T>(correctType);
            for (Node* n : correctType) {

                if (!n->IsEnabled()) {
                    created = n->GetComponent<T>();
                    break;
                }
            }
        }
        if(!created) {

            Node* spawnedNode{ MC->scene_->CreateChild(T::GetTypeStatic().ToString()) };
            created = spawnedNode->CreateComponent<T>();
            spawnedNode->SetEnabledRecursive(false);
        }

        return created;
    }

    template <class T> int CountActive()
    {
        int count{0};
        PODVector<Node*> result{};
        MC->scene_->GetChildrenWithComponent<T>(result);

        for (Node* r : result) {

            if (r->IsEnabled()) ++count;
        }
        return count;
    }

private:
    float sincePredator_;
    float predatorInterval_;
    int predatorWave_;
    int predatorCounter_;

    float sinceReaper_;
    float reaperInterval_;

    float sinceTR3B_;
    float TR3BInterval_;

    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);

    void Activate();
    void Deactivate();
    void Restart();
};

#endif // SPAWNMASTER_H
