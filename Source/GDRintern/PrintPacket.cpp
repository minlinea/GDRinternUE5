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
}

// Called when the game starts or when spawned
void APrintPacket::BeginPlay()
{
	Super::BeginPlay();
	ConnectServer();
	//pCClient.ClientInit();
	//pCClient.ClientConnect();
}

// Called every frame
void APrintPacket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("%s"), *FString(to_string(pCClient.GetBallPlace())));

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
	//pCClient.Instance().SetActiveState(false);
	//pCClient.Instance().SetBallPlace(BALLPLACE::OB);
}

void APrintPacket::ConnectServer()
{
	sSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FString address = TEXT("192.168.245.130");
	int32 port = 8989;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	bool connected = sSocket->Connect(*addr);

}

void APrintPacket::ClientRecv()
{
	uint8* recvdata;
	recvdata = (uint8*)FMemory::Malloc(8);
	uint32 Size = 8;
	int Read = 0;
	sSocket->Recv(recvdata, 8, Read);

	Packet pt;
	FMemory::Memmove(&pt, recvdata, 8);

	FMemory::Free(recvdata);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
		FString::Printf(TEXT("Input Value : packettype : %d, packetsize : %d"),
			(unsigned int)pt.GetType(), (unsigned int)pt.GetSize()));
	//test
}

#undef LOCTEXT_NAMESPACE

//쓰레드 사용관련
//FThread::FThread(threadname, ConnectServer);
//FRunnableThread* Thread
//satic FSocketListenThread* Instance;
/*FRunnableThread* tThread;

tThread->Create((FRunnable*)tThread, TEXT("FSocketListenThreadRunnable"));
tThread->WaitForCompletion();*/

//bool StartListening()
//{
//	if (Thread != nullptr) return false; //Already running
//	Thread = FRunnableThread::Create(this, TEXT("FSocketListenThread"), FSocketListenThread::ThreadStackSize, TPri_AboveNormal);
//	return (Thread != nullptr)
//}

//void StopListening()
//{
//	if (Thread != nullptr) {
//		// Ask your thread to stop somehow
//		Thread->WaitForCompletion();
//		delete Thread;
//		Thread = nullptr;
//	}
//}