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

public:	
	// Sets default values for this actor's properties
	ADataManagement();
	~ADataManagement();



	UFUNCTION(BlueprintCallable)
	bool ConnectServer();

	/*
	* 소켓의 연결 여부를 통해 판단
	* return true	: 연결되어 있던 상태에서의 접속 종료 요청 -> 연결 끊는 동작 수행(소켓, 스레드)
	* return false	: 연결되어 있지 않던 상태에서의 접속 종료 요청 -> 아무것도 하지 않음
	*/
	UFUNCTION(BlueprintCallable)
	bool DisconnectServer();

	UFUNCTION(BlueprintCallable)
	bool isConnected();

	UFUNCTION(BlueprintCallable)
	void SendPacket(const FString& type);
	
	template <class P, class PACKETDATA>
	void PushSendQueue(PACKETDATA data)
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


private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FString m_sServerAddress;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 m_iServerPort;
	FSocket* m_sSocket;

	SendThread* m_tSend;
	RecvThread* m_tRecv;
};

#define GIdata static_cast<UGameInstanceData*>(GetGameInstance())