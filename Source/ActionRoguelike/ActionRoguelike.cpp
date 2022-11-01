// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionRoguelike.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ActionRoguelike, "ActionRoguelike" );

static TAutoConsoleVariable<bool> CVarDebugDrawTags(TEXT("su.DebugPrintActions"), false, TEXT("Enable Debug String for ActionComponent."), ECVF_Cheat);