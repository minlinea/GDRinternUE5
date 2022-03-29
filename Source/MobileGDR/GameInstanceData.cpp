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
*	true�� ���� �� ActiveState �̹��� ������Ʈ �� SendPacet("ActiveState") ����
*	true ����
*	1. ActiveState�� ��ȭ�� �ִ� ���,
*	2. ActiveStateLock�� Ȱ��ȭ �� ���(�� �����Ͱ� �Ѿ�� 3�� ��ݻ����� ���)
*/
bool UGameInstanceData::CheckActiveState()
{
	bool check = this->m_bActiveState;	//��ȭ ���� �Ǵ� ���� ����

	if (false == this->m_bActiveStateLock)	//��ȿ�� Ŭ�� �˻�
	{
		if (CLUBSETTING::DRIVER == this->m_eClub)	// ����̹� + Ƽ
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
			//isIn���� ������ Sort�� ���� �ʾ� possible ���ڴ� BALLPLACE ������ ������ ��.

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
*	UGameInstanceData::CheckActiveState�� ��� ������ m_bActiveStateLock ����ġ �Լ�
*	DataManagement:: ManageData���� ActiveState ���� �� ȣ���ϸ�, 3�� Ÿ�̸� �Լ��� �����Ͽ� 3�� �ڿ� ��ȣ��
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