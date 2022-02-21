// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "packet.h"
#include "../TestThread.h"
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

	//CClient pCClient;
	FSocket* sSocket;
	int32 packetsize;
	int32 packettype;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString ServerAddress;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 ServerPort;

	TestThread tThread;
	FRunnableThread* ptest;
public:	
	// Sets default values for this actor's properties
	APrintPacket();
	~APrintPacket();

	UFUNCTION(BlueprintCallable)
	void UpdateText();

	UFUNCTION(BlueprintCallable)
	void ChangeData();

	UFUNCTION(BlueprintCallable)
	void ConnectServer();

	UFUNCTION(BlueprintCallable)
	void ClientRecv();

	UFUNCTION(BlueprintCallable)
	void MakeThread();

	UFUNCTION(BlueprintCallable)
	void KillThread();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
