// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBall.h"
float size(float data);
// Sets default values
AGolfBall::AGolfBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->m_sdShotData = ShotDataInfo{ 0,0,0,0,0,0,0,0,0 };
	

	//UStaticMesh
	this->m_cSphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	this->SetRootComponent(this->m_cSphereMesh);
	//this->m_cSphereMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	this->m_cSphereMesh->SetStaticMesh(SphereMeshAsset.Object);


	this->m_cSphereMesh->SetWorldScale3D(FVector{ 0.5f, 0.5f, 0.5f });
	this->m_cSphereMesh->SetSimulatePhysics(true);
	this->m_cSphereMesh->SetMassOverrideInKg(NAME_None, 0.045f, true);
	this->m_cSphereMesh->SetLinearDamping(0.01f);
	this->m_cSphereMesh->SetAngularDamping(0.01f);
	this->m_cSphereMesh->SetGenerateOverlapEvents(false);
	//this->m_cSphereMesh->SetScale

	this->m_vAddpower = FVector{ 0.f,0.f,0.f };

	this->m_Pawn = CreateDefaultSubobject<APawn>(TEXT("PAWN"));
	this->m_Pawn->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	this->m_CameraActor = CreateDefaultSubobject < ACameraActor>(TEXT("CAMERA"));
	this->m_CameraActor->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>CameraMeshAsset(TEXT("StaticMesh'/Engine/EditorMeshes/MatineeCam_SM.MatineeCam_SM'"));



	//this->m_CameraActor->SetStaticMesh(CameraMeshAsset.Object);
}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	
	
	PhysicsUpdate(DeltaTime);
}

void AGolfBall::PhysicsUpdate(float DeltaTime)
{

	if (this->m_vAddpower.Size() > 5.0f)
	{
		if (0.1f <= size(this->m_vAddpower.Z))
		{
			this->m_vAddpower.Z -= DeltaTime * 9.8;
			if (0.1f <= size(this->m_vAddpower.Y))
			{
				this->m_vAddpower.Y -= DeltaTime;
			}
		}
		else
		{
			this->m_vAddpower.Z = 0.f;
			this->m_vAddpower.Y = 0.f;
		}
		if (1.f <= this->m_vAddpower.X)
		{
			this->m_vAddpower.X -= 11.7 * DeltaTime;
		}
		else
		{
			this->m_vAddpower.X = 0;
		}
		this->m_cSphereMesh->AddForce(this->m_vAddpower);
	}
	else
	{
		this->m_vAddpower = FVector{ 0.f,0.f,0.f };
	}
}

float size(float data)
{
	return sqrt(data * data);
}

