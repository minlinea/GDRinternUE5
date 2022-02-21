// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../TestThread.h"
#include "SocketSubsystem.h"
#include "C:\Program Files\Epic Games\UE_5.0EA\Engine\Source\Runtime\Networking\Public\Interfaces\IPv4\IPv4Address.h"
#include "C:\Program Files\Epic Games\UE_5.0EA\Engine\Source\Runtime\Networking\Public\Interfaces\IPv4\IPv4SubnetMask.h"
#include "PrintPacket.generated.h"


UCLASS()
class GDRINTERN_API APrintPacket : public AActor
{
	GENERATED_BODY()
	
	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* BallPlaceText;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ShotDataText;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ActiveStateText;

	TestThread* tThread;

	bool m_bActiveState;
	BALLPLACE m_eBallPlace;
	ShotData m_sdShotData;
	TEESETTING m_eTee;
	CLUBSETTING m_eClub;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString m_sServerAddress;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 m_iServerPort;
	FSocket* m_sSocket;

public:	
	// Sets default values for this actor's properties
	APrintPacket();
	~APrintPacket();

	UFUNCTION(BlueprintCallable)
	void MakeThread();

	UFUNCTION(BlueprintCallable)
	void KillThread();

	UFUNCTION(BlueprintCallable)
	void UpdateText();

	UFUNCTION(BlueprintCallable)
	void ConnectServer();
	void CheckQueue();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void SetShotData(const ShotData& sd)
	{
		this->m_sdShotData = sd;
	}
	void SetShotData(uint8* sd)
	{
		FMemory::Memmove(&m_sdShotData, sd, sizeof(ShotData));
	}
	void SetTeeSetting(const TEESETTING& tee)
	{
		this->m_eTee = tee;
	}
	void SetTeeSetting(uint8* tee)
	{
		FMemory::Memmove(&m_eTee, tee, sizeof(TEESETTING));
	}
	void SetClubSetting(const CLUBSETTING& club)
	{
		this->m_eClub = club;
	}
	void SetClubSetting(uint8* club)
	{
		FMemory::Memmove(&m_eClub, club, sizeof(CLUBSETTING));
	}
	void SetBallPlace(const BALLPLACE& place)
	{
		this->m_eBallPlace = place;
	}
	void SetBallPlace(uint8* ballplace)
	{
		FMemory::Memmove(&m_eBallPlace, ballplace, sizeof(BALLPLACE));
	}
	void SetActiveState(const bool& activestate)
	{
		this->m_bActiveState = activestate;
	}
	void SetActiveState(uint8* activestate)
	{
		FMemory::Memmove(&m_bActiveState, activestate, sizeof(bool));
	}


	const TEESETTING GetTeeSetting()
	{
		return this->m_eTee;
	}
	const CLUBSETTING GetClubSetting()
	{
		return this->m_eClub;
	}
	const ShotData GetShotData()
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


};
