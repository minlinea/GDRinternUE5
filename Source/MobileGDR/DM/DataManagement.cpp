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

	//UGameViewportClient tp;
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

/*
*	RecvThread�� ��� ť ������ ó��
*/
void ADataManagement::CheckQueue()
{
	if (nullptr != this->m_tSend)
	{
		if (false == this->m_tRecv->GetQueue().empty())		//���𰡰� ����ִ� ���(��Ŷ�� recv�� ���)
		{
			auto packetqueue = this->m_tRecv->GetQueue();	//Recv ť ����
			for (auto packet = packetqueue.front(); true != packetqueue.empty(); )
			{
				packet = packetqueue.front();

				ManageData(packet);

				delete packet;
				packetqueue.pop();
			}
			this->m_tRecv->SetQueue(packetqueue);			//Recv ť ����
		}
	}
}

/*
*	��Ŷ�� Ÿ�Կ� ���� ������ ó��
*	UI Update ������ ���� m_bUpdate true�� ����, BallPlace, ActiveState, ShotData�� �ƴ� ��� �ٷ� false�� ����
*	PT_ActiveState�� ��� ActiveStateFunction ȣ��
*/
void ADataManagement::ManageData(Packet* pt)
{
	this->m_bUpdate = true;

	if (PACKETTYPE::PT_BallPlace == pt->GetType())
	{
		GIdata->SetBallPlace(static_cast<PacketBallPlace*>(pt)->GetData());

		PushSendQueue<Packet>(PACKETTYPE::PT_BallPlaceRecv);	//Send ������ ť�� recv ��Ŷ �߰�
	}
	else if (PACKETTYPE::PT_ShotData == pt->GetType())
	{
		GIdata->SetShotData(static_cast<PacketShotDataInfo*>(pt)->GetData());

		GIdata->SetShotDataUpdate(false);

		PushSendQueue<Packet>(PACKETTYPE::PT_ShotDataRecv);
	}
	else if (PACKETTYPE::PT_ActiveState == pt->GetType())		//Send ������ ť�� recv ��Ŷ �߰�
	{
		GIdata->SetActiveState(static_cast<PacketActiveState*>(pt)->GetData());

		GIdata->ActiveStateFunction();		//UGameInstanceData m_bActiveStateLock ����ġ �Լ� ȣ��
		GetWorldTimerManager().SetTimer(GIdata->m_hActiveStateTimer, GIdata, &UGameInstanceData::ActiveStateFunction, 1.0f, false, 3.0f);	//3�� �ڿ� �ش� �Լ� ȣ��

		PushSendQueue<Packet>(PACKETTYPE::PT_ActiveStateRecv);	//Send ������ ť�� recv ��Ŷ �߰�
	}
	else
	{
		this->m_bUpdate = false;	//UI Update�� �ʿ���� Ÿ���� ��Ŷ�� ���
	}
}

/*
*	�𸮾� ������ �����ϴ� TCP ��� ���� �Լ�
*	Connect ���� �� true ����
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
			MakeThread();			//Send, Recv������ ����
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
* ������ ���� ���θ� ���� �Ǵ�
* return true	: ����Ǿ� �ִ� ���¿����� ���� ���� ��û -> ���� ���� ���� ����(����, ������)
* return false	: ����Ǿ� ���� �ʴ� ���¿����� ���� ���� ��û -> �ƹ��͵� ���� ����
*/
bool ADataManagement::DisconnectServer()
{	
	if (nullptr != this->m_sSocket)		//Socket�� nullptr��� �Ʒ��� ������ ������� ����
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
*	�������� ���� ���� Ȯ��
*	true ���� : Recv �����尡 ���� ���� ���
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
*	Send, Recv ������ ����
*	������ OB�� �ٲ� ��, ���� �ۿ� ������ Club, Tee, ActiveState�� ������ ����
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


/*
*	BP���� ����ϴ� ��Ŷ ���� �Լ�, type�� ���� ����
*	SendThread ť�� ���� ��Ŷ �߰�
*/
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
	else	//���ǵǾ� ���� ���� Ÿ��
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow,
			FString::Printf(TEXT("SendPacket Input UnknownType %s"), *type), true, FVector2D{ 2.f, 2.f });
	}
}
