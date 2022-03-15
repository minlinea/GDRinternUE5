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
	* ������ ���� ���θ� ���� �Ǵ�
	* return true	: ����Ǿ� �ִ� ���¿����� ���� ���� ��û -> ���� ���� ���� ����(����, ������)
	* return false	: ����Ǿ� ���� �ʴ� ���¿����� ���� ���� ��û -> �ƹ��͵� ���� ����
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
	* ���� ���� ���� Send, Recv ������ ����
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