// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceThread.h"
#include "Packet.h"
#include "RecvThread.h"
#include "SendThread.h"
#include "Interfaces\IPv4\IPv4Address.h"
#include "SocketSubsystem.h"
#include "DataManagement.generated.h"

UCLASS()
class MOBILEGDR_API ADataManagement : public AActor
{
	GENERATED_BODY()
	
private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString m_sServerAddress;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 m_iServerPort;
	FSocket* m_sSocket;

	SendThread* m_tSend;
	RecvThread* m_tRecv;

	bool m_bActiveState;
	BALLPLACE m_eBallPlace;
	ShotData m_sdShotData;
	TEESETTING m_eTee;
	CLUBSETTING m_eClub;

public:	
	// Sets default values for this actor's properties
	ADataManagement();
	~ADataManagement();
	/*
	* socket 생성, ip parsing, connect
	* 현재 모바일 빌드 시에 SCS_NotConnected가 통과되지 않는 문제가 있음
	*/
	UFUNCTION(BlueprintCallable)
	void ConnectServer();		

	UFUNCTION(BlueprintCallable)
	void DisconnectServer();

	UFUNCTION(BlueprintCallable)
	void SendClubSetting();

	UFUNCTION(BlueprintCallable)
	void SendTeeSetting();

	UFUNCTION(BlueprintCallable)
	void SendActiveState();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	template <class P, class PACKETDATA>
	void SendPacket(PACKETDATA data);
	
	/*
	* 서버 연결 이후 Send, Recv 스레드 생성
	* Send : SendThread / Recv : RecvThread
	*/
	void MakeThread();
	void CheckQueue();
	void ManageData(Packet* pt);


	
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


	UFUNCTION(BlueprintCallable)
	const FString GetFStringTeeSetting()
	{
		return FString(to_string(this->m_eTee));
	}
	UFUNCTION(BlueprintCallable)
	const FString GetFStringClubSetting()
	{
		return FString(to_string(this->m_eClub));
	}
	UFUNCTION(BlueprintCallable)
	const FString GetFStringActiveState()
	{
		return FString(to_string(this->m_bActiveState));
	}
	UFUNCTION(BlueprintCallable)
	const FString GetFStringBallPlace()
	{
		return FString(to_string(this->m_eBallPlace));
	}

	UFUNCTION(BlueprintCallable)
	const float GetFStringBallSpeed()
	{
		return this->m_sdShotData.ballspeed;
	}
	UFUNCTION(BlueprintCallable)
	const float GetFStringLaunchAngle()
	{
		return this->m_sdShotData.launchangle;
	}
	UFUNCTION(BlueprintCallable)
	const float GetFStringLaunchDirection()
	{
		return this->m_sdShotData.launchdirection;
	}
	UFUNCTION(BlueprintCallable)
	const float GetFStringHeadspeed()
	{
		return this->m_sdShotData.headspeed;
	}
	UFUNCTION(BlueprintCallable)
	const int GetFStringBackSpin()
	{
		return this->m_sdShotData.backspin;
	}
	UFUNCTION(BlueprintCallable)
	const int GetFStringSideSpin()
	{
		return this->m_sdShotData.sidespin;
	}


	UFUNCTION(BlueprintCallable)
	void SetClubSetting(const FString& club)
	{
		if (FString(to_string(CLUBSETTING::DRIVER)) == club)
		{
			this->m_eClub = CLUBSETTING::DRIVER;
		}
		else if (FString(to_string(CLUBSETTING::WOOD)) == club)
		{
			this->m_eClub = CLUBSETTING::WOOD;
		}
		else if (FString(to_string(CLUBSETTING::IRON)) == club)
		{
			this->m_eClub = CLUBSETTING::IRON;
		}
	}

	UFUNCTION(BlueprintCallable)
	void SetTeeSetting(const FString& tee)
	{
		if (FString(to_string(TEESETTING::T30)) == tee)
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
	}
};
