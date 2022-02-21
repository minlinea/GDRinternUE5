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

	packetsize = 0;
	packettype = 0;

	ServerAddress = TEXT("192.168.206.126");
	ServerPort = 8989;

	ptest = nullptr;
}

APrintPacket::~APrintPacket()
{
	tThread.Stop();
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

void APrintPacket::ChangeData()
{

}

void APrintPacket::ConnectServer()
{
	sSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	int32 port = 8989;
	FIPv4Address ip;
	FIPv4Address::Parse(ServerAddress, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	bool connected = sSocket->Connect(*addr);

}

void APrintPacket::ClientRecv()
{
	Packet pt;
	uint8* recvdata{ (uint8*)FMemory::Malloc(sizeof(Packet)) };
	int32 Read{ sizeof(Packet) };
	
	sSocket->Recv(recvdata, sizeof(Packet), Read);

	FMemory::Memmove(&pt, recvdata, sizeof(Packet));

	FMemory::Free(recvdata);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		FString::Printf(TEXT("Input Value : packettype : %d, packetsize : %d"),
			(unsigned int)pt.GetType(), (unsigned int)pt.GetSize()), true, FVector2D{ 2.f, 2.f });
}

void APrintPacket::MakeThread()
{
	
	FRunnableThread::Create(&tThread, TEXT("Create TestThread"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Wait.")));
	ConnectServer();
	//tThread = 
	//tThread->Init();
	//SendThread->
	//void StopListening()
	//{
	//	if (Thread != nullptr) {
	//		// Ask your thread to stop somehow
	//		Thread->WaitForCompletion();
	//		delete Thread;
	//		Thread = nullptr;
	//	}
	//}
}

void APrintPacket::KillThread()
{
	tThread.Stop();
}
#undef LOCTEXT_NAMESPACE

