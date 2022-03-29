// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DM/packet.h"
#include "GameInstanceData.generated.h"

/**
 * MobileGDR의 GameInstance
 * 게임 시작 버튼 시 생성이 이루어지며, 초기화 한 뒤 프로그램 종료까지 데이터 저장
 * 
 * Init : UGameInstance에서 생성 시 호출되는 함수 (기본 값 정의)
 */
UCLASS()
class MOBILEGDR_API UGameInstanceData : public UGameInstance
{
	GENERATED_BODY()
	
	bool m_bActiveState;
	bool m_bActiveStateLock;

	BALLPLACE m_eBallPlace;
	ShotDataInfo m_sdShotData;
	TEESETTING m_eTee;
	CLUBSETTING m_eClub;
	
	virtual void Init() override;

	bool m_bShotDataUpdate;

public:
	FTimerHandle m_hActiveStateTimer;

	UFUNCTION(BlueprintCallable)
	void ActiveStateFunction();

	//return : CheckActiveState를 통해 m_bActiveState가 바뀌었다면 true
	UFUNCTION(BlueprintCallable)
	bool CheckActiveState();

	UFUNCTION(BlueprintPure)
	bool GetShotDataUpdate()
	{
		return this->m_bShotDataUpdate;
	}
	UFUNCTION(BlueprintCallable)
	void SetShotDataUpdate(const bool& shotdataupdate)
	{
		this->m_bShotDataUpdate = shotdataupdate;
	}

	UFUNCTION(BlueprintPure)
	bool GetActiveStateLock()
	{
		return this->m_bActiveStateLock;
	}
	UFUNCTION(BlueprintCallable)
	void SetActiveStateLock(const bool& asl)
	{
		this->m_bActiveStateLock = asl;
	}

	void SetShotData(const ShotDataInfo& sd)
	{
		this->m_sdShotData = sd;
	}
	void SetTeeSetting(const TEESETTING& tee)
	{
		this->m_eTee = tee;
	}
	void SetClubSetting(const CLUBSETTING& club)
	{
		this->m_eClub = club;
	}
	void SetBallPlace(const BALLPLACE& place)
	{
		this->m_eBallPlace = place;
	}

	const TEESETTING GetTeeSetting()
	{
		return this->m_eTee;
	}
	const CLUBSETTING GetClubSetting()
	{
		return this->m_eClub;
	}
	const ShotDataInfo GetShotData()
	{
		return this->m_sdShotData;
	}
	const BALLPLACE GetBallPlace()
	{
		return this->m_eBallPlace;
	}


	UFUNCTION(BlueprintPure)
	const FString GetFStringTeeSetting()	{return FString(to_string(this->m_eTee));}
	UFUNCTION(BlueprintPure)
	const FString GetFStringClubSetting()	{return FString(to_string(this->m_eClub));}
	UFUNCTION(BlueprintPure)
	const FString GetFStringBallPlace()		{ return FString(to_string(this->m_eBallPlace)); }
	UFUNCTION(BlueprintPure)
	const FString GetFStringActiveState()	{ return FString(to_string(this->m_bActiveState)); }

	UFUNCTION(BlueprintPure)
	const float GetBallSpeed()	{return this->m_sdShotData.ballspeed;}
	UFUNCTION(BlueprintPure)
	const float GetBallPitch()	{return this->m_sdShotData.ballpitch;}
	UFUNCTION(BlueprintPure)
	const float GetBallYaw()	{return this->m_sdShotData.ballyaw;}
	UFUNCTION(BlueprintPure)
	const int GetSpinBack()		{return this->m_sdShotData.spinback;}
	UFUNCTION(BlueprintPure)
	const int GetSpinSide()		{return this->m_sdShotData.spinside;}
	UFUNCTION(BlueprintPure)
	const float GetClubSpeed()	{return this->m_sdShotData.clubspeed;}
	UFUNCTION(BlueprintPure)
	const float GetClubPath()	{return this->m_sdShotData.clubpath;}
	UFUNCTION(BlueprintPure)
	const float GetClubFace()	{return this->m_sdShotData.clubface;}
	UFUNCTION(BlueprintPure)
	const bool GetActiveState() { return this->m_bActiveState; }

	UFUNCTION(BlueprintCallable)
	void SetClubSetting(const int& club)
	{
		if (static_cast<int>(CLUBSETTING::DRIVER) == club)
		{
			this->m_eClub = CLUBSETTING::DRIVER;
		}
		else if (static_cast<int>(CLUBSETTING::IRON) == club)
		{
			this->m_eClub = CLUBSETTING::IRON;
		}
		else if (static_cast<int>(CLUBSETTING::WEDGE) == club)
		{
			this->m_eClub = CLUBSETTING::WEDGE;
		}
		else if (static_cast<int>(CLUBSETTING::PUTTER) == club)
		{
			this->m_eClub = CLUBSETTING::PUTTER;
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetTeeSetting(const int& tee)
	{
		if (static_cast<int>(TEESETTING::T25) == tee)
		{
			this->m_eTee = TEESETTING::T25;
		}
		else if (static_cast<int>(TEESETTING::T30) == tee)
		{
			this->m_eTee = TEESETTING::T30;
		}
		else if (static_cast<int>(TEESETTING::T35) == tee)
		{
			this->m_eTee = TEESETTING::T35;
		}
		else if (static_cast<int>(TEESETTING::T40) == tee)
		{
			this->m_eTee = TEESETTING::T40;
		}
		else if (static_cast<int>(TEESETTING::T45) == tee)
		{
			this->m_eTee = TEESETTING::T45;
		}
		else if (static_cast<int>(TEESETTING::T50) == tee)
		{
			this->m_eTee = TEESETTING::T50;
		}
		else if (static_cast<int>(TEESETTING::T55) == tee)
		{
			this->m_eTee = TEESETTING::T55;
		}
		else if (static_cast<int>(TEESETTING::T60) == tee)
		{
			this->m_eTee = TEESETTING::T60;
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetActiveState(const bool& tf)
	{
		this->m_bActiveState = tf;
	}
};
