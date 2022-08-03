// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/BaseGameMode.h"
#include "BasePlayer.h"
#include "BasePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGameMode, All, All);

ABaseGameMode::ABaseGameMode()
{
	DefaultPawnClass = ABasePlayer::StaticClass();
	PlayerStateClass = ABasePlayerState::StaticClass();
}
