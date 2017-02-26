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

#include "dhcam.h"

void DHCam::RegisterObject(Context *context)
{
    context->RegisterFactory<DHCam>();
}

DHCam::DHCam(Context *context):
    LogicComponent(context)
{
}

void DHCam::OnNodeSet(Node *node)
{ if (!node) return;

    node_->SetPosition(Vector3(0.0f, 34.0f, -13.0f));
    node_->SetRotation(Quaternion(75.0f, 0.0f, 0.0f));

    camera_ = node_->CreateComponent<Camera>();
    camera_->SetFov(23.0f);
    camera_->SetFarClip(42.0f);
    camera_->SetNearClip(0.05f);

    SetupViewport();
}

void DHCam::SetupViewport()
{

    //Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, MC->scene_, camera_));
    viewport_ = viewport;

    //Add anti-asliasing and bloom
    effectRenderPath_ = viewport_->GetRenderPath()->Clone();
    effectRenderPath_->Append(CACHE->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
    effectRenderPath_->SetEnabled("FXAA3", true);
    effectRenderPath_->Append(CACHE->GetResource<XMLFile>("PostProcess/BloomHDR.xml"));
    effectRenderPath_->SetShaderParameter("BloomHDRThreshold", 0.23f);
    effectRenderPath_->SetShaderParameter("BloomHDRMix", Vector2(1.75f, 1.25f));
    effectRenderPath_->SetEnabled("BloomHDR", true);

    Renderer* renderer{ GetSubsystem<Renderer>() };
    viewport_->SetRenderPath(effectRenderPath_);
    renderer->SetViewport(0, viewport_);
}

Vector3 DHCam::GetWorldPosition()
{
    return node_->GetWorldPosition();
}

Quaternion DHCam::GetRotation()
{
    return node_->GetRotation();
}
