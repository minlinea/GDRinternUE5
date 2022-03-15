// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceData.h"

void UGameInstanceData::Init()
{
	Super::Init();

	this->m_bActiveState = false;
	this->m_eBallPlace = BALLPLACE::OUTOFBOUND;
	this->m_sdShotData = ShotDataInfo{ 0,0,0,0,0,0,0,0,0 };
	this->m_eTee = TEESETTING::T25;
	this->m_eClub = CLUBSETTING::DRIVER;
	this->m_bActiveStateLock = false;
}

bool UGameInstanceData::CheckActiveState()
{
	bool check = this->m_bActiveState;
	if (false == this->m_bActiveStateLock)
	{
		if (CLUBSETTING::DRIVER == this->m_eClub && BALLPLACE::TEE == this->m_eBallPlace)
		{
			this->m_bActiveState = true;
		}
		else
		{
			std::vector<BALLPLACE> possible = { BALLPLACE::FAIRWAY, BALLPLACE::ROUGH, BALLPLACE::BUNKER };

			this->m_bActiveState = isIn(possible, this->m_eBallPlace);
		}
	}
	
	if (check != this->m_bActiveState || true == this->m_bActiveStateLock)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UGameInstanceData::ActiveStateFunction()
{
	if (false == this->m_bActiveStateLock)
	{
		this->m_bActiveStateLock = true;
	}
	else
	{
		this->m_bActiveStateLock = false;
	}
}