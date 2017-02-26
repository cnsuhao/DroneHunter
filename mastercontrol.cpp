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

#include "mastercontrol.h"
#include "inputmaster.h"
#include "spawnmaster.h"
#include "dhcam.h"
#include "building.h"
#include "character.h"
#include "rock.h"
#include "player.h"
#include "predator.h"
#include "reaper.h"
#include "tr3b.h"

URHO3D_DEFINE_APPLICATION_MAIN(MasterControl);

MasterControl* MasterControl::instance_ = NULL;

MasterControl* MasterControl::GetInstance()
{
    return MasterControl::instance_;
}

MasterControl::MasterControl(Context *context):
    Application(context)
{
    instance_ = this;
}

void MasterControl::Setup()
{
    SetRandomSeed(TIME->GetSystemTime());

    engineParameters_["WindowTitle"] = "Drone Hunter: Rage Against the Machine";
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs")+"LucKeyTemplate.log";
    engineParameters_["ResourcePaths"] = "Data;CoreData;Resources";
    engineParameters_["WindowIcon"] = "icon.png";
//    engineParameters_["FullScreen"] = false;
//    engineParameters_["WindowWidth"] = 960;
//    engineParameters_["WindowHeight"] = 540;
}
void MasterControl::Start()
{
    DHCam::RegisterObject(context_);
    Building::RegisterObject(context_);
    Character::RegisterObject(context_);
    Rock::RegisterObject(context_);

    Predator::RegisterObject(context_);
    Reaper::RegisterObject(context_);
    TR3B::RegisterObject(context_);

    context_->RegisterSubsystem(new InputMaster(context_));
    context_->RegisterSubsystem(new SpawnMaster(context_));

    CreateScene();

    //Play music
//    Sound* music{ CACHE->GetResource<Sound>("Resources/Music/Urho - Disciples of Urho.ogg") };
//    music->SetLooped(true);
//    Node* musicNode{ scene_->CreateChild("Music") };
//    SoundSource* musicSource{ musicNode->CreateComponent<SoundSource>() };
//    musicSource->SetSoundType(SOUND_MUSIC);
//    musicSource->Play(music);

    GetSubsystem<SpawnMaster>()->Activate();
}
void MasterControl::Stop()
{
    engine_->DumpResources(true);
}
void MasterControl::Exit()
{
    engine_->Exit();
}


void MasterControl::CreateScene()
{
    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();
    camera_ = scene_->CreateChild("Camera")->CreateComponent<DHCam>();
    CreateLights();

    Node* desertNode{ scene_->CreateChild("Desert")};
    StaticModel* desertModel{ desertNode->CreateComponent<StaticModel>() };
    desertModel->SetModel(GetModel("Desert"));
    desertModel->SetMaterial(MC->GetMaterial("VCol"));
    desertNode->CreateComponent<RigidBody>();
    desertNode->CreateComponent<CollisionShape>()->SetBox(Vector3(23.0f, 0.1f, 23.0f), Vector3::DOWN * 0.05f);

    Node* buildingNode{ scene_->CreateChild("Platform") };
    buildingNode->CreateComponent<Building>();

    for (int p : {1}){
        players_.Push(SharedPtr<Player>(new Player(p, context_)));

        Character* character{ GetSubsystem<SpawnMaster>()->Create<Character>() };
        character->Set(Vector3(0.0f, 0.1f, -10.0f));

        GetSubsystem<InputMaster>()->SetPlayerControl(GetPlayer(p), character);
    }
}

void MasterControl::CreateLights()
{
    //Add a directional light to the world. Enable cascaded shadows on it
    Node* downardsLightNode{ scene_->CreateChild("DirectionalLight") };
    downardsLightNode->SetPosition(Vector3(-2.0f, 10.0f, -5.0f));
    downardsLightNode->LookAt(Vector3(0.0f, 0.0f, 0.0f));
    Light* downwardsLight{ downardsLightNode->CreateComponent<Light>() };
    downwardsLight->SetLightType(LIGHT_DIRECTIONAL);
    downwardsLight->SetBrightness(0.42);
    downwardsLight->SetColor(Color(0.8f, 0.9f, 0.95f));
    downwardsLight->SetCastShadows(true);
    downwardsLight->SetShadowIntensity(0.23f);
    downwardsLight->SetShadowBias(BiasParameters(0.000025f, 0.5f));
    downwardsLight->SetShadowCascade(CascadeParameters(1.0f, 5.0f, 23.0f, 100.0f, 0.8f));

    //Add a directional light to the world. Enable cascaded shadows on it
    /*Node* upwardsLightNode = world.scene->CreateChild("DirectionalLight");
    upwardsLightNode->SetPosition(Vector3(3.0f, 2.0f, 5.0f));
    upwardsLightNode->LookAt(Vector3(0.0f, 0.0f, 0.0f));
    Light* upwardsLight = upwardsLightNode->CreateComponent<Light>();
    upwardsLight->SetCastShadows(true);
    upwardsLight->SetLightType(LIGHT_DIRECTIONAL);
    upwardsLight->SetBrightness(0.34);
    upwardsLight->SetColor(Color(0.23f, 0.666f, 1.0f));

    //Create a point light. Enable cascaded shadows on it
    Node* pointLightNode_ = world.scene->CreateChild("MovingLight");
    pointLightNode_->SetPosition(Vector3(0.0f, 23.0f, 0.0f));
    Light* pointLight = pointLightNode_->CreateComponent<Light>();
    pointLight->SetCastShadows(true);
    pointLight->SetLightType(LIGHT_POINT);
    pointLight->SetBrightness(0.1f);
    pointLight->SetRange(42.0f);
    pointLight->SetColor(Color(1.0f, 0.0f, 0.0f));*/
}

Vector<SharedPtr<Player> > MasterControl::GetPlayers() const {
    return players_;
}
Player* MasterControl::GetPlayer(int playerId) const
{
    for (Player* p : players_) {

        if (p->GetPlayerId() == playerId)
            return p;
    }
    return nullptr;
}

Material* MasterControl::GetMaterial(String name) const
{
    return CACHE->GetResource<Material>("Materials/" + name + ".xml");
}
Model* MasterControl::GetModel(String name) const
{
    return CACHE->GetResource<Model>("Models/" + name + ".mdl");
}
Texture* MasterControl::GetTexture(String name) const
{
    return CACHE->GetResource<Texture>("Textures/" + name + ".png");
}
Sound* MasterControl::GetMusic(String name) const {
    Sound* song{ CACHE->GetResource<Sound>("Music/" + name + ".ogg") };
    song->SetLooped(true);
    return song;
}
Sound* MasterControl::GetSample(String name) const {
    Sound* sample{ CACHE->GetResource<Sound>("Samples/" + name + ".ogg") };
    sample->SetLooped(false);
    return sample;
}

float MasterControl::Sine(const float freq, const float min, const float max, const float shift)
{
    float phase{SinePhase(freq, shift)};
    float add{0.5f * (min + max)};
    return LucKey::Sine(phase) * 0.5f * (max - min) + add;
}
float MasterControl::Cosine(const float freq, const float min, const float max, const float shift)
{
    return Sine(freq, min, max, shift + 0.25f);
}
float MasterControl::SinePhase(float freq, float shift)
{
    return M_PI * 2.0f * (freq * scene_->GetElapsedTime() + shift);
}
