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

#include "tr3b.h"

void TR3B::RegisterObject(Context* context)
{
    context->RegisterFactory<TR3B>();
}

TR3B::TR3B(Context* context) : Drone(context)
{

}

void TR3B::OnNodeSet(Node* node)
{ (void)node;

    Drone::OnNodeSet(node);
    collisionShape_->SetTriangleMesh(MC->GetModel("TR-3B_collision"));

    StaticModel* model{ node_->CreateComponent<StaticModel>() };
    model->SetModel(MC->GetModel("TR-3B"));
    model->SetMaterial(MC->GetMaterial("VColOutline"));
    model->SetCastShadows(true);
}

void TR3B::Set(Vector3 position)
{
    SceneObject::Set(position);
    if (position.x_ < 0.0f)
        direction_ = Vector3::RIGHT;
    else
        direction_ = Vector3::LEFT;
}

void TR3B::Update(float timeStep)
{
    node_->Translate(direction_ * timeStep, TS_WORLD);

    node_->Rotate(Quaternion(5.0f * timeStep, Vector3::UP));
}




