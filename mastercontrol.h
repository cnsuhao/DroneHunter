/* Drone Hunter
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

#ifndef MASTERCONTROL_H
#define MASTERCONTROL_H

#include <Urho3D/Urho3D.h>
#include "luckey.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class DHCam;
class Player;
class InputMaster;

class MasterControl : public Application
{
    URHO3D_OBJECT(MasterControl, Application);
    friend class InputMaster;
public:
    MasterControl(Context* context);
    static MasterControl* GetInstance();

    SharedPtr<Scene> scene_;
    SharedPtr<DHCam> camera_;

    Vector< SharedPtr<Player> > players_;
    Vector< SharedPtr<Player> > GetPlayers() const;
    Player* GetPlayer(int playerId) const;

    Material* GetMaterial(String name) const;
    Model* GetModel(String name) const;
    Texture* GetTexture(String name) const;
    Sound* GetMusic(String name) const;
    Sound* GetSample(String name) const;

    virtual void Setup();
    virtual void Start();
    virtual void Stop();

    void Exit();
    void CreateLights();

    float Sine(const float freq, const float min, const float max, const float shift);
    float Cosine(const float freq, const float min, const float max, const float shift);
private:
    static MasterControl* instance_;
    Node* movingLightNode_;

    void CreateScene();

    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    float SinePhase(float freq, float shift);
};

#endif // MASTERCONTROL_H
