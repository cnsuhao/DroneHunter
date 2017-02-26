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

#include "spawnmaster.h"
#include "player.h"
#include "rock.h"

#include "character.h"

void Character::RegisterObject(Context* context)
{
    context->RegisterFactory<Character>();
}

Character::Character(Context* context) : Controllable(context),
  dashInterval_{0.01f},
  shotInterval_{0.23f}
{
}

void Character::OnNodeSet(Node* node)
{ if (!node) return;

    Controllable::OnNodeSet(node_);

    rigidBody_->SetMass(1.0f);
    rigidBody_->SetAngularFactor(Vector3::UP);
    rigidBody_->SetFriction(0.0f);
    rigidBody_->SetLinearDamping(0.95f);
    rigidBody_->SetAngularDamping(5.0f);
    rigidBody_->SetRestitution(0.0f);
    rigidBody_->SetLinearFactor(Vector3::RIGHT);

    collisionShape_->SetCapsule(0.23f, 0.32f, Vector3::UP * 0.16f);
    model_->SetModel(MC->GetModel("Character"));
    model_->SetMaterial(MC->GetMaterial("VColOutline"));

}

void Character::Update(float timeStep)
{
    sinceDash_ += timeStep;
    sinceShot_ += timeStep;

    rigidBody_->ApplyForce(Vector3::RIGHT * move_.x_ * timeStep * 512.0f);

    //Update rotation according to direction of the player's movement.
    Vector3 velocity{ rigidBody_->GetLinearVelocity() };
    Vector3 lookDirection{ velocity + 2.0f * aim_ };
    Quaternion rotation{ node_->GetWorldRotation() };
    Quaternion aimRotation{ rotation };
    aimRotation.FromLookRotation(lookDirection);
    node_->SetRotation(rotation.Slerp(aimRotation, 7.0f * timeStep * velocity.Length()));

    if (actions_[1] && sinceShot_ > shotInterval_)
        Shoot();
}

void Character::Shoot()
{
    GetSubsystem<SpawnMaster>()->Create<Rock>()->Set(node_->GetPosition() + Vector3::UP);

    sinceShot_ = 0.0f;
}
