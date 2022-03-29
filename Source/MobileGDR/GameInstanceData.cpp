// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceData.h"

void UGameInstanceData::Init()
{
	Super::Init();

	this->m_bActiveStateLock = false;
	this->m_bActiveState = false;

	this->m_eBallPlace = BALLPLACE::OUTOFBOUND;
	this->m_sdShotData = ShotDataInfo{ 0,0,0,0,0,0,0,0,0 };
	this->m_eTee = TEESETTING::T25;
	this->m_eClub = CLUBSETTING::DRIVER;

	this->m_bShotDataUpdate = false;

}

/*
*	true로 리턴 시 ActiveState 이미지 업데이트 및 SendPacet("ActiveState") 진행
*	true 조건
*	1. ActiveState의 변화가 있는 경우,
*	2. ActiveStateLock이 활성화 된 경우(샷 데이터가 넘어와 3초 잠금상태인 경우)
*/
bool UGameInstanceData::CheckActiveState()
{
	bool check = this->m_bActiveState;	//변화 여부 판단 저장 변수

	if (false == this->m_bActiveStateLock)	//유효한 클럽 검사
	{
		if (CLUBSETTING::DRIVER == this->m_eClub)	// 드라이버 + 티
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
		else
		{
			std::vector<BALLPLACE> possible = { BALLPLACE::FAIRWAY, BALLPLACE::ROUGH, BALLPLACE::BUNKER };
			//isIn에서 별도로 Sort는 하지 않아 possible 인자는 BALLPLACE 순서를 맞춰줄 것.

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

/*
*	UGameInstanceData::CheckActiveState의 잠금 변수인 m_bActiveStateLock 스위치 함수
*	DataManagement:: ManageData에서 ActiveState 수신 시 호출하며, 3초 타이머 함수를 설정하여 3초 뒤에 재호출
*	
*/
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