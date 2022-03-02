// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceData.h"

void UGameInstanceData::Init()
{
	Super::Init();

	this->m_bActiveState = false;
	this->m_eBallPlace = BALLPLACE::OUTOFBOUND;
	this->m_sdShotData = ShotDataInfo{ 0,0,0,0,0,0,0,0,0 };
	this->m_eTee = TEESETTING::T30;
	this->m_eClub = CLUBSETTING::DRIVER;
	this->m_bActiveStateLock = false;
}

void UGameInstanceData::CheckActiveState()
{
	if (false == this->m_bActiveStateLock)
	{
		if (CLUBSETTING::DRIVER == this->m_eClub)
		{
			if (BALLPLACE::TEE == this->m_eBallPlace)
			{
				this->m_bActiveState = true;
			}
			else
			{
				this->m_bActiveState = false;
			}
		}
		else if (CLUBSETTING::PUTTER == this->m_eClub)
		{
			if (BALLPLACE::GREEN == this->m_eBallPlace)
			{
				this->m_bActiveState = true;
			}
			else
			{
				this->m_bActiveState = false;
			}
		}
		else
		{
			std::vector<BALLPLACE> possible = { BALLPLACE::FAIRWAY, BALLPLACE::ROUGH, BALLPLACE::BUNKER };

			this->m_bActiveState = isIn(possible, this->m_eBallPlace);
		}
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
		CheckActiveState();
	}
}