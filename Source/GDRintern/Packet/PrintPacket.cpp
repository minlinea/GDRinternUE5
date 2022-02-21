// Fill out your copyright notice in the Description page of Project Settings.


#include "PrintPacket.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"


#define LOCTEXT_NAMESPACE "TCPIP"

// Sets default values
APrintPacket::APrintPacket()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	DummyRoot->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	DummyRoot->SetWorldRotation(FRotator(0.f, 0.f, 0.f));

	// Create static mesh component
	ShotDataText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ShotDataText0"));
	ShotDataText->SetRelativeLocation(FVector(300.f, -300.f, 0.f));
	ShotDataText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	ShotDataText->SetWorldScale3D(FVector(1.f, 2.5f, 2.5f));
	ShotDataText->SetText(FText::Format(LOCTEXT("ScoreFmt", "a: {0}"), FText::AsNumber(1)));
	ShotDataText->SetupAttachment(DummyRoot);

	// Create static mesh component
	BallPlaceText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BallPlaceText0"));
	BallPlaceText->SetRelativeLocation(FVector(300.f, 300.f, 0.f));
	BallPlaceText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	BallPlaceText->SetWorldScale3D(FVector(1.f, 2.5f, 2.5f));
	BallPlaceText->SetText(FText::Format(LOCTEXT("ScoreFmt", "b: {0}"), FText::AsNumber(2)));
	BallPlaceText->SetupAttachment(DummyRoot);

	// Create static mesh component
	ActiveStateText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ActiveStateText0"));
	ActiveStateText->SetRelativeLocation(FVector(-300.f, -300.f, 0.f));
	ActiveStateText->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	ActiveStateText->SetWorldScale3D(FVector(1.f, 2.5f, 2.5f));
	ActiveStateText->SetText(FText::Format(LOCTEXT("ScoreFmt", "c: {0}"), FText::AsNumber(3)));
	ActiveStateText->SetupAttachment(DummyRoot);

	this->m_eBallPlace = BALLPLACE::TEE;
	this->m_bActiveState = false;
	this->m_sdShotData = ShotData{ 0,0,0,0,0,0,0 };
	this->m_eClub = CLUBSETTING::WOOD;
	this->m_eTee = TEESETTING::T30;

	this->m_sServerAddress = TEXT("192.168.245.130");
	this->m_iServerPort = 8989;
	this->m_sSocket = nullptr;
	
	this->tThread = nullptr;
}

APrintPacket::~APrintPacket()
{
	if (this->tThread != nullptr)
	{
		if (true != this->tThread->m_qPacket.empty())
		{
			for (auto p = this->tThread->m_qPacket.front(); true != this->tThread->m_qPacket.empty(); )
			{
				p = this->tThread->m_qPacket.front();
				delete p;
				this->tThread->m_qPacket.pop();
			}
		}

		this->tThread->Stop();
		delete this->tThread;
	}

	if (nullptr != this->m_sSocket)
	{
		if (SCS_Connected == m_sSocket->GetConnectionState())
		{
			m_sSocket->Close();
		}
		this->m_sSocket = nullptr;
	}

}
// Called when the game starts or when spawned
void APrintPacket::BeginPlay()
{
	Super::BeginPlay();
	//ConnectServer();
}

// Called every frame
void APrintPacket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckQueue();
	//UpdateText();
}

void APrintPacket::UpdateText()
{
	
	//ShotDataText->SetText(FText::Format(LOCTEXT("ScoreFmt", "ShotDataText: {%s}"),
	//	FText::FromString(FString(to_string(pCClient.GetBallPlace())))));
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(to_string(pCClient.GetBallPlace())));

	//ActiveStateText->SetText(FText::Format(LOCTEXT("ScoreFmt", "ActiveStateText: {0}"),
	//	FText::FromString(FString(to_string(pCClient.Instance().GetActiveState())))));

	//BallPlaceText->SetText(FText::Format(LOCTEXT("ScoreFmt", "BallPlaceText: {0}"),
	//	FText::FromString(FString(to_string(pCClient.Instance().GetBallPlace())))));
}

void APrintPacket::ConnectServer()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		FString::Printf(TEXT("ConnectServer")), true, FVector2D{ 2.f, 2.f });

	this->m_sSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FIPv4Address ip;
	FIPv4Address::Parse(m_sServerAddress, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, m_iServerPort);

	if (true == m_sSocket->Connect(*addr))
	{
		MakeThread();

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect OK")), true, FVector2D{ 2.f, 2.f });
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("Connect Fail")), true, FVector2D{ 2.f, 2.f });
	}
	return;
}

void APrintPacket::MakeThread()
{
	tThread = new TestThread(m_sSocket);
	FRunnableThread::Create(tThread, TEXT("TestThread"));
}

void APrintPacket::KillThread()
{
	tThread->Stop();
	if (nullptr != tThread)
	{
		delete tThread;
	}
	tThread = nullptr;
}

void APrintPacket::CheckQueue()
{
	if (nullptr != tThread)
	{
		if (true != tThread->m_qPacket.empty())
		{
			for (auto p = tThread->m_qPacket.front(); true != tThread->m_qPacket.empty(); )
			{
				p = tThread->m_qPacket.front();

				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,
					FString::Printf(TEXT("Input Value : packettype : %s, packetsize : %d"),
						*FString(to_string(p->GetType())), (unsigned int)p->GetSize()), true, FVector2D{ 2.f, 2.f });

				delete p;
				tThread->m_qPacket.pop();
			}
		}
	}
}
#undef LOCTEXT_NAMESPACE

