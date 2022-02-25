// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../GameInstanceData.h"
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

public:	
	// Sets default values for this actor's properties
	ADataManagement();
	~ADataManagement();

	/*
	* socket 생성, ip parsing, connect
	* 현재 모바일 빌드 시에 SCS_NotConnected가 통과되지 않는 문제가 있음
	*/
	UFUNCTION(BlueprintCallable)
	bool ConnectServer();		
	UFUNCTION(BlueprintCallable)
	void DisconnectServer();
	UFUNCTION(BlueprintCallable)
	bool isConnected();


	UFUNCTION(BlueprintCallable)
	void SendClubSetting();
	UFUNCTION(BlueprintCallable)
	void SendTeeSetting();
	UFUNCTION(BlueprintCallable)
	void SendActiveState();
	

	template <class P, class PACKETDATA>
	void SendPacket(PACKETDATA data)
	{
		this->m_tSend->GetQueue().push(new P(data));
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	/*
	* 서버 연결 이후 Send, Recv 스레드 생성
	* Send : SendThread / Recv : RecvThread
	*/
	void MakeThread();
	void CheckQueue();
	void ManageData(Packet* pt);

};

#define GIdata static_cast<UGameInstanceData*>(GetGameInstance())