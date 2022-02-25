// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManagement.h"
#include <vector>
// Sets default values
ADataManagement::ADataManagement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->m_sServerAddress = TEXT("192.168.245.130");
	this->m_iServerPort = PORT;
	this->m_sSocket = nullptr;

	this->m_tSend = nullptr;
	this->m_tRecv = nullptr;

	this->m_bActiveState = false;
	this->m_eBallPlace = BALLPLACE::OUTOFBOUND;
	this->m_sdShotData = ShotDataInfo{ 0,0,0,0,0,0,0,0,0 };
	this->m_eTee = TEESETTING::T30;
	this->m_eClub = CLUBSETTING::DRIVER;
}

ADataManagement::~ADataManagement()
{
	DisconnectServer();
}

// Called when the game starts or when spawned
void ADataManagement::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADataManagement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckQueue();
}

//RecvThread에 담긴 큐 데이터 처리
void ADataManagement::CheckQueue()
{
	if (nullptr != this->m_tSend)
	{
		if (0 != this->m_tRecv->GetQueue().size())
		{
			auto packetqueue = this->m_tRecv->GetQueue();
			for (auto packet = packetqueue.front(); true != packetqueue.empty(); )
			{
				packet = packetqueue.front();

				ManageData(packet);

				delete packet;
				packetqueue.pop();
			}
			this->m_tRecv->SetQueue(packetqueue);
		}
	}
	
}

void ADataManagement::CheckActiveState()
{
	if (CLUBSETTING::DRIVER == this->m_eClub)
	{
		if (BALLPLACE::TEE == this->m_eBallPlace)
		{
			this->m_bActiveState = true;
		}
		else
		{
			this->m_bActiveState = false;
		}
	}
	else if (CLUBSETTING::PUTTER == this->m_eClub)
	{
		if (BALLPLACE::GREEN == this->m_eBallPlace)
		{
			this->m_bActiveState = true;
		}
		else
		{
			this->m_bActiveState = false;
		}
	}
	else
	{
		std::vector<BALLPLACE> possible = { BALLPLACE::FAIRWAY, BALLPLACE::ROUGH, BALLPLACE::BUNKER };
		
		this->m_bActiveState = isIn(possible,this->m_eBallPlace);
	}
}

void ADataManagement::ManageData(Packet* pt)
{
	if (PACKETTYPE::PT_BallPlace == pt->GetType())
	{
		SetBallPlace(static_cast<PacketBallPlace*>(pt)->GetData());
		SendPacket<Packet>(PACKETTYPE::PT_BallPlaceRecv);

		CheckActiveState();
	}
	else if (PACKETTYPE::PT_ShotData == pt->GetType())
	{
		SetShotData(static_cast<PacketShotDataInfo*>(pt)->GetData());
		SendPacket<Packet>(PACKETTYPE::PT_ShotDataRecv);
	}
	else if (PACKETTYPE::PT_ActiveState == pt->GetType())
	{
		SetActiveState(static_cast<PacketActiveState*>(pt)->GetData());
		SendPacket<Packet>(PACKETTYPE::PT_ActiveStateRecv);
	}
	else if (PACKETTYPE::PT_ConnectCheck == pt->GetType())
	{
		//SendPacket<Packet>(PACKETTYPE::PT_ActiveStateRecv);
	}
}

void ADataManagement::ConnectServer()
{
	this->m_sSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FIPv4Address ip;
	FIPv4Address::Parse(this->m_sServerAddress, ip);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)
		->CreateInternetAddr(ip.Value, this->m_iServerPort);

	if (true == this->m_sSocket->Connect(*addr))
	{
		MakeThread();			//Send, Recv스레드 생성

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(TEXT("Connect OK")), true, FVector2D{ 2.f, 2.f });
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("Connect Fail")), true, FVector2D{ 2.f, 2.f });
	}
}

void ADataManagement::DisconnectServer()
{
	if (nullptr != this->m_sSocket)
	{
		this->m_sSocket->Close();
	}

	if (nullptr != this->m_tSend)
	{
		this->m_tSend->Stop();
	}

	if (nullptr != this->m_tRecv)
	{
		this->m_tRecv->Stop();
	}
}

void ADataManagement::MakeThread()
{
	this->m_tSend = new SendThread(this->m_sSocket);
	FRunnableThread::Create(this->m_tSend, TEXT("SendThread"));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	//	FString::Printf(TEXT("Create SendThread")), true, FVector2D{ 2.f, 2.f });

	this->m_tRecv = new RecvThread(this->m_sSocket);
	FRunnableThread::Create(this->m_tRecv, TEXT("RecvThread"));
}

template <class P, class PACKETDATA>
void ADataManagement::SendPacket(PACKETDATA data)
{
	this->m_tSend->GetQueue().push(new P(data));
}

void ADataManagement::SendClubSetting()
{
	SendPacket<PacketClubSetting>(this->m_eClub);
}
void ADataManagement::SendTeeSetting()
{
	SendPacket<PacketTeeSetting>(this->m_eTee);
}
void ADataManagement::SendActiveState()
{
	SendPacket<PacketActiveState>(this->m_bActiveState);
}
