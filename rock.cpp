/* Drone Hunter
// Copyright (C) 2017 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// Commercial licenses are available through frode@lindeijer.nl
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

#include "rock.h"

void Rock::RegisterObject(Context* context)
{
    context->RegisterFactory<Rock>();
}

Rock::Rock(Context* context) : SceneObject(context)
{

}

void Rock::OnNodeSet(Node* node)
{ (void)node;
    StaticModel* model{ node_->CreateComponent<StaticModel>() };
    model->SetModel(MC->GetModel("Rock"));
    model->SetCastShadows(true);
    model->SetMaterial(MC->GetMaterial("VColOutline"));
    node_->CreateComponent<RigidBody>()->SetMass(0.5f);
    node_->CreateComponent<CollisionShape>()->SetSphere(0.1f);
}

void Rock::Set(Vector3 position)
{
    SceneObject::Set(position);
    node_->SetRotation(Quaternion(Random(360.0f), Random(360.0f), Random(360.0f)));
    node_->SetScale(Random(0.9f, 1.1f));

    RigidBody* rigidBody{ node_->GetComponent<RigidBody>() };
    rigidBody->ResetForces();
    rigidBody->ApplyImpulse(Vector3(0.0f, 3.4f, 4.2f));

    spin_ = Quaternion(Random(-13.0f, 13.0f), Random(-13.0f, 13.0f), Random(-13.0f, 13.0f));
}

void Rock::Update(float timeStep)
{
    node_->Rotate(spin_ * timeStep);
}
