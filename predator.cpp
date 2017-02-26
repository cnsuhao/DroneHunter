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

#include "predator.h"

void Predator::RegisterObject(Context* context)
{
    context->RegisterFactory<Predator>();
}

Predator::Predator(Context* context) : Drone(context)
{

}

void Predator::OnNodeSet(Node* node)
{ if (!node) return;

    Drone::OnNodeSet(node);
    collisionShape_->SetSphere(1.0f);

    StaticModel* model{ node_->CreateComponent<StaticModel>() };
    model->SetModel(MC->GetModel("Predator"));
    model->SetMaterial(MC->GetMaterial("VColOutline"));
    model->SetCastShadows(true);

}

void Predator::Update(float timeStep)
{
    node_->Translate(2.3f * Vector3::FORWARD * timeStep);
}




