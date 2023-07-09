// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIW_PlayerHUD.h"

#include "Components/ProgressBar.h"


void UUIW_PlayerHUD::SetLifeBar(float percent)
{
	lifeBar->SetPercent(percent);
}

void UUIW_PlayerHUD::SetManaBar(float percent)
{
	manaBar->SetPercent(percent);

}