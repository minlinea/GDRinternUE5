// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DM/packet.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GolfBall.generated.h"

UCLASS()
class MOBILEGDR_API AGolfBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGolfBall();

	void PhysicsUpdate(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	ShotDataInfo m_sdShotData;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_cSphereMesh;

	UPROPERTY(EditAnywhere)
	APawn* m_Pawn;

	UPROPERTY(EditAnywhere)
	ACameraActor* m_CameraActor;

	UPROPERTY(EditAnywhere)
	FVector m_vAddpower;

	UFUNCTION(BlueprintCallable)
	void SetAddPower(FVector power)
	{
		this->m_vAddpower = power;
	}

	UFUNCTION(BlueprintPure)
	FVector GetAddPower()
	{
		return this->m_vAddpower;
	}
};
