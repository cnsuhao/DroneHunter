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

#include "spawnmaster.h"

#include "predator.h"
#include "reaper.h"
#include "tr3b.h"

SpawnMaster::SpawnMaster(Context* context):
    Object(context),
    sincePredator_{0.0f},
    predatorInterval_{5.0f},
    predatorWave_{1},
    predatorCounter_{0},
    sinceReaper_{0.0f},
    reaperInterval_{10.0f},
    sinceTR3B_{0.0f},
    TR3BInterval_{TR3BINTERVAL}
{
}

void SpawnMaster::Activate()
{
//    AUDIO->SetMasterGain(SOUND_EFFECT, 0.0f);
//    AUDIO->SetMasterGain(SOUND_EFFECT, 1.0f);

    SubscribeToEvent(E_SCENEUPDATE, URHO3D_HANDLER(SpawnMaster, HandleSceneUpdate));
}
void SpawnMaster::Deactivate()
{
    UnsubscribeFromAllEvents();
}
void SpawnMaster::Clear()
{
}

void SpawnMaster::Restart()
{
    Clear();
    Activate();
}

Vector3 SpawnMaster::SpawnPoint()
{
    return Vector3(Random(-5.0f, 5.0f), -42.0f, Random(-5.0f, 5.0f));
}

void SpawnMaster::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{ (void)eventType;

    const float timeStep{ eventData[SceneUpdate::P_TIMESTEP].GetFloat() };

    sincePredator_ += timeStep;
    sinceReaper_ += timeStep;
    sinceTR3B_ += timeStep;

    if (sincePredator_ > predatorInterval_) {
        sincePredator_ = 0.0f;

        Create<Predator>()->Set(PREDATORSPAWNPOINT);
    }

    if (sinceReaper_ > reaperInterval_) {
        sinceReaper_ = 0.0f;
        reaperInterval_ = Max(reaperInterval_ * 0.9f, 1.0f);

        Create<Reaper>()->Set(REAPERSPAWNPOINT);
    }

    if (sinceTR3B_ > TR3BInterval_) {
        sinceTR3B_ = 0.0f;
        TR3BInterval_ = TR3BINTERVAL;

        Create<TR3B>()->Set(TR3BSPAWNPOINT);
    }

}
