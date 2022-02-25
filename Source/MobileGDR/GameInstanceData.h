// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DM/packet.h"
#include "GameInstanceData.generated.h"

/**
 * 
 */
UCLASS()
class MOBILEGDR_API UGameInstanceData : public UGameInstance
{
	GENERATED_BODY()
	
	bool m_bActiveState;
	BALLPLACE m_eBallPlace;
	ShotDataInfo m_sdShotData;
	TEESETTING m_eTee;
	CLUBSETTING m_eClub;

	virtual void Init() override;

public:

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateText();

	void CheckActiveState();

	void SetShotData(const ShotDataInfo& sd)
	{
		this->m_sdShotData = sd;
	}
	void SetShotData(uint8* sd)
	{
		FMemory::Memmove(&m_sdShotData, sd, sizeof(ShotDataInfo));
		UpdateText();
	}
	void SetTeeSetting(const TEESETTING& tee)
	{
		this->m_eTee = tee;
	}
	void SetTeeSetting(uint8* tee)
	{
		FMemory::Memmove(&m_eTee, tee, sizeof(TEESETTING));
		UpdateText();
	}
	void SetClubSetting(const CLUBSETTING& club)
	{
		this->m_eClub = club;
	}
	void SetClubSetting(uint8* club)
	{
		FMemory::Memmove(&m_eClub, club, sizeof(CLUBSETTING));
		UpdateText();
	}
	void SetBallPlace(const BALLPLACE& place)
	{
		this->m_eBallPlace = place;
	}
	void SetBallPlace(uint8* ballplace)
	{
		FMemory::Memmove(&m_eBallPlace, ballplace, sizeof(BALLPLACE));
		UpdateText();
	}
	void SetActiveState(const bool& activestate)
	{
		this->m_bActiveState = activestate;
	}
	void SetActiveState(uint8* activestate)
	{
		FMemory::Memmove(&m_bActiveState, activestate, sizeof(bool));
		UpdateText();
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
	const bool GetActiveState()
	{
		return this->m_bActiveState;
	}


	UFUNCTION(BlueprintPure)
	const FString GetFStringTeeSetting()	{return FString(to_string(this->m_eTee));}
	UFUNCTION(BlueprintPure)
	const FString GetFStringClubSetting()	{return FString(to_string(this->m_eClub));}
	UFUNCTION(BlueprintPure)
	const FString GetFStringActiveState()	{return FString(to_string(this->m_bActiveState));}
	UFUNCTION(BlueprintPure)
	const FString GetFStringBallPlace()		{return FString(to_string(this->m_eBallPlace));}

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

	UFUNCTION(BlueprintCallable)
	void SetClubSetting(const FString& club)
	{
		if (FString(to_string(CLUBSETTING::DRIVER)) == club)
		{
			this->m_eClub = CLUBSETTING::DRIVER;
		}
		else if (FString(to_string(CLUBSETTING::IRON)) == club)
		{
			this->m_eClub = CLUBSETTING::IRON;
		}
		else if (FString(to_string(CLUBSETTING::WEDGE)) == club)
		{
			this->m_eClub = CLUBSETTING::WEDGE;
		}
		else if (FString(to_string(CLUBSETTING::PUTTER)) == club)
		{
			this->m_eClub = CLUBSETTING::PUTTER;
		}
		CheckActiveState();
	}

	UFUNCTION(BlueprintCallable)
	void SetTeeSetting(const FString& tee)
	{
		if (FString(to_string(TEESETTING::T25)) == tee)
		{
			this->m_eTee = TEESETTING::T25;
		}
		else if (FString(to_string(TEESETTING::T30)) == tee)
		{
			this->m_eTee = TEESETTING::T30;
		}
		else if (FString(to_string(TEESETTING::T35)) == tee)
		{
			this->m_eTee = TEESETTING::T35;
		}
		else if (FString(to_string(TEESETTING::T40)) == tee)
		{
			this->m_eTee = TEESETTING::T40;
		}
		else if (FString(to_string(TEESETTING::T45)) == tee)
		{
			this->m_eTee = TEESETTING::T45;
		}
		else if (FString(to_string(TEESETTING::T50)) == tee)
		{
			this->m_eTee = TEESETTING::T50;
		}
		else if (FString(to_string(TEESETTING::T55)) == tee)
		{
			this->m_eTee = TEESETTING::T55;
		}
		else if (FString(to_string(TEESETTING::T60)) == tee)
		{
			this->m_eTee = TEESETTING::T60;
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetActiveState(const FString& tf)
	{
		if (FString("True") == tf)
		{
			this->m_bActiveState = true;
		}
		else if (FString("False") == tf)
		{
			this->m_bActiveState = false;
		}
		//UpdateText();
	}
};
