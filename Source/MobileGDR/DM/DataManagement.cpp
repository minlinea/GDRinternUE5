// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManagement.h"
#include <vector>
#include "Kismet/GameplayStatics.h"
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

/*
*	패킷의 타입에 따라 데이터 처리
*	UI Update 실행을 위한 m_bUpdate true로 변경, BallPlace, ActiveState, ShotData가 아닌 경우 바로 false로 변경
*	PT_ActiveState인 경우 ActiveStateFunction 호출
*/
void ADataManagement::ManageData(Packet* pt)
{
	this->m_bUpdate = true;

	if (PACKETTYPE::PT_BallPlace == pt->GetType())
	{
		GIdata->SetBallPlace(static_cast<PacketBallPlace*>(pt)->GetData());

		PushSendQueue<Packet>(PACKETTYPE::PT_BallPlaceRecv);
	}
	else if (PACKETTYPE::PT_ShotData == pt->GetType())
	{
		GIdata->SetShotData(static_cast<PacketShotDataInfo*>(pt)->GetData());

		PushSendQueue<Packet>(PACKETTYPE::PT_ShotDataRecv);
	}
	else if (PACKETTYPE::PT_ActiveState == pt->GetType())
	{
		GIdata->SetActiveState(static_cast<PacketActiveState*>(pt)->GetData());

		GIdata->ActiveStateFunction();		//UGameInstanceData m_bActiveStateLock 스위치 함수 호출
		GetWorldTimerManager().SetTimer(GIdata->m_hActiveStateTimer, GIdata, &UGameInstanceData::ActiveStateFunction, 1.0f, false, 3.0f);	//3초 뒤에 해당 함수 호출

		PushSendQueue<Packet>(PACKETTYPE::PT_ActiveStateRecv);
	}
	else
	{
		this->m_bUpdate = false;
	}
}

/*
*	언리얼 내에서 지원하는 TCP 통신 연결 함수
*/
bool ADataManagement::ConnectServer()
{
	if (false == isConnected())
	{
		this->m_sSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)
				->CreateSocket(NAME_Stream, TEXT("default"), false);

		FIPv4Address ip;
		FIPv4Address::Parse(this->m_sServerAddress, ip);

		TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)
				->CreateInternetAddr(ip.Value, this->m_iServerPort);

		if (true == this->m_sSocket->Connect(*addr))
		{
			MakeThread();			//Send, Recv스레드 생성
			return true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			//	FString::Printf(TEXT("Connect Fail")), true, FVector2D{ 2.f, 2.f });
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
* 소켓의 연결 여부를 통해 판단
* return true	: 연결되어 있던 상태에서의 접속 종료 요청 -> 연결 끊는 동작 수행(소켓, 스레드)
* return false	: 연결되어 있지 않던 상태에서의 접속 종료 요청 -> 아무것도 하지 않음
*/
bool ADataManagement::DisconnectServer()
{	
	if (nullptr != this->m_sSocket)
	{
		this->m_sSocket->Close();
		this->m_sSocket = nullptr;

		if (nullptr != this->m_tRecv)
		{
			this->m_tRecv->Exit();
			this->m_tRecv = nullptr;
		}
		if (nullptr != this->m_tSend)
		{
			this->m_tSend->Exit();
			this->m_tSend = nullptr;
		}
		return true;
	}
	else
	{
		return false;
	}
}

/*
*	서버와의 접속 여부 확인
*	true 조건 : Recv 스레드가 동작 중인 경우
*/
bool ADataManagement::isConnected()
{
	if (nullptr != this->m_tRecv)
	{
		if (true == this->m_tRecv->GetRun())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
*	Send, Recv 스레드 생성
*	데이터 OB로 바꾼 후, 현재 앱에 설정된 Club, Tee, ActiveState를 센서로 전달
*/
void ADataManagement::MakeThread()
{
	this->m_tSend = new SendThread(this->m_sSocket);
	FRunnableThread::Create(this->m_tSend, TEXT("SendThread"));

	this->m_tRecv = new RecvThread(this->m_sSocket);
	FRunnableThread::Create(this->m_tRecv, TEXT("RecvThread"));

	GIdata->SetBallPlace(BALLPLACE::OUTOFBOUND);
	SendPacket("ClubSetting");
	SendPacket("TeeSetting");
	SendPacket("ActiveState");
}



void ADataManagement::SendPacket(const FString& type)
{
	if (FString("ClubSetting") == type)
	{
		PushSendQueue<PacketClubSetting>(GIdata->GetClubSetting());
	}
	else if (FString("TeeSetting") == type)
	{
		PushSendQueue<PacketTeeSetting>(GIdata->GetTeeSetting());
	}
	else if (FString("ActiveState") == type)
	{
		PushSendQueue<PacketActiveState>(GIdata->GetActiveState());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
			FString::Printf(TEXT("SendPacket Input UnknownType %s"), *type), true, FVector2D{ 2.f, 2.f });
	}
}
