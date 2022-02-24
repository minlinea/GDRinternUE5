#pragma once

#include <iostream>

//////////////////////////////////////////////////////////////////////////////////
// define

#define SERVER_IP "192.168.206.126"
#define PORT 8989
#define PACKETHEADER sizeof(Packet)


//////////////////////////////////////////////////////////////////////////////////
// enum

inline const char* to_string(const bool& t);
inline const char* to_string(const bool& t)
{
	if (true == t)	return "TRUE";
	else			return "FALSE";
}

enum class TEESETTING {
	T25,
	T30,
	T35,
	T40,
	T45,
	T50,
	T55,
	T60
};
inline std::ostream& operator << (std::ostream& os, const TEESETTING& t);
inline std::ostream& operator << (std::ostream& os, const TEESETTING& t)
{
	if (TEESETTING::T25 == t)		std::cout << "T25";
	else if (TEESETTING::T30 == t)	std::cout << "T30";
	else if (TEESETTING::T35 == t)	std::cout << "T35";
	else if (TEESETTING::T40 == t)	std::cout << "T40";
	else if (TEESETTING::T45 == t)	std::cout << "T45";
	else if (TEESETTING::T50 == t)	std::cout << "T50";
	else if (TEESETTING::T55 == t)	std::cout << "T55";
	else if (TEESETTING::T60 == t)	std::cout << "T60";
	return os;
}
inline const char* to_string(const TEESETTING& t);
inline const char* to_string(const TEESETTING& t)
{
	if (TEESETTING::T25 == t)		return "T25";
	else if (TEESETTING::T30 == t)	return "T30";
	else if (TEESETTING::T35 == t)	return "T35";
	else if (TEESETTING::T40 == t)	return "T40";
	else if (TEESETTING::T45 == t)	return "T45";
	else if (TEESETTING::T50 == t)	return "T50";
	else if (TEESETTING::T55 == t)	return "T55";
	else if (TEESETTING::T60 == t)	return "T60";
	return "NONE";
}

enum class CLUBSETTING {
	DRIVER,
	IRON,
	WEDGE,
	PUTTER
};
inline std::ostream& operator << (std::ostream& os, const CLUBSETTING& t);
inline std::ostream& operator << (std::ostream& os, const CLUBSETTING& t)
{
	if (CLUBSETTING::DRIVER == t)		std::cout << "DRIVER";
	else if (CLUBSETTING::IRON == t)	std::cout << "IRON";
	else if (CLUBSETTING::WEDGE == t)	std::cout << "WEDGE";
	else if (CLUBSETTING::PUTTER == t)	std::cout << "PUTTER";
	return os;
}
inline const char* to_string(const CLUBSETTING& t);
inline const char* to_string(const CLUBSETTING& t)
{
	if (CLUBSETTING::DRIVER == t)	return "DRIVER";
	else if (CLUBSETTING::IRON == t)	return "IRON";
	else if (CLUBSETTING::WEDGE == t)	return "WEDGE";
	else if (CLUBSETTING::PUTTER == t)	return "PUTTER";
	return "NONE";
}

enum class BALLPLACE
{
	FAIRWAY,
	ROUGH,
	BUNKER,
	TEE,
	GREEN,
	WAIT,
	SLEEP,
	OUTOFBOUND
};
inline std::ostream& operator << (std::ostream& os, const BALLPLACE& t);
inline std::ostream& operator << (std::ostream& os, const BALLPLACE& t)
{
	if (BALLPLACE::FAIRWAY == t)	std::cout << "PAIRWAY";
	else if (BALLPLACE::ROUGH == t)	std::cout << "ROUGH";
	else if (BALLPLACE::BUNKER == t)std::cout << "BUNKER";
	else if (BALLPLACE::TEE == t)	std::cout << "TEE";
	else if (BALLPLACE::GREEN == t)	std::cout << "GREEN";
	else if (BALLPLACE::WAIT == t)	std::cout << "WAIT";
	else if (BALLPLACE::SLEEP == t)	std::cout << "SLEEP";
	else if (BALLPLACE::OUTOFBOUND == t)	std::cout << "OUTOFBOUND";
	return os;
}
inline const char* to_string(const BALLPLACE& t);
inline const char* to_string(const BALLPLACE& t)
{
	if (BALLPLACE::FAIRWAY == t)		return "PAIRWAY";
	else if (BALLPLACE::ROUGH == t)		return "ROUGH";
	else if (BALLPLACE::BUNKER == t)	return "BUNKER";
	else if (BALLPLACE::TEE == t)		return "TEE";
	else if (BALLPLACE::GREEN == t)		return "GREEN";
	else if (BALLPLACE::WAIT == t)		return "WAIT";
	else if (BALLPLACE::SLEEP == t)		return "SLEEP";
	else if (BALLPLACE::OUTOFBOUND == t)return "OUTOFBOUND";
	return "NONE";
}




enum class PACKETTYPE {
	PT_ActiveState,
	PT_ActiveStateRecv,
	PT_BallPlace,
	PT_BallPlaceRecv,
	PT_ConnectCheck,
	PT_Disconnect,
	PT_ClubSetting,
	PT_ClubSettingRecv,
	PT_ShotData,
	PT_ShotDataRecv,
	PT_TeeSetting,
	PT_TeeSettingRecv,
	PT_None
};
inline std::ostream& operator << (std::ostream& os, const PACKETTYPE& t);
inline std::ostream& operator << (std::ostream& os, const PACKETTYPE& t)
{
	if (PACKETTYPE::PT_ActiveState == t)			std::cout << "PT_ActiveState";
	else if (PACKETTYPE::PT_ActiveStateRecv == t)	std::cout << "PT_ActiveStateRecv";
	else if (PACKETTYPE::PT_BallPlace == t)			std::cout << "PT_BallPlace";
	else if (PACKETTYPE::PT_BallPlaceRecv == t)		std::cout << "PT_BallPlaceRecv";
	else if (PACKETTYPE::PT_ConnectCheck == t)		std::cout << "PT_ConnectCheck";
	else if (PACKETTYPE::PT_Disconnect == t)		std::cout << "PT_Disconnect";
	else if (PACKETTYPE::PT_ClubSetting == t)		std::cout << "PT_ClubSetting";
	else if (PACKETTYPE::PT_ClubSettingRecv == t)	std::cout << "PT_ClubSettingRecv";
	else if (PACKETTYPE::PT_ShotData == t)			std::cout << "PT_ShotData";
	else if (PACKETTYPE::PT_ShotDataRecv == t)		std::cout << "PT_ShotDataRecv";
	else if (PACKETTYPE::PT_TeeSetting == t)		std::cout << "PT_TeeSetting";
	else if (PACKETTYPE::PT_TeeSettingRecv == t)	std::cout << "PT_TeeSettingRecv";
	else if (PACKETTYPE::PT_None == t)				std::cout << "PT_None";
	else											std::cout << "None Type Message";

	return os;
}
inline const char* to_string(const PACKETTYPE& t);
inline const char* to_string(const PACKETTYPE& t)
{
	if (PACKETTYPE::PT_ActiveState == t)			return "PT_ActiveState";
	else if (PACKETTYPE::PT_ActiveStateRecv == t)	return "PT_ActiveStateRecv";
	else if (PACKETTYPE::PT_BallPlace == t)			return "PT_BallPlace";
	else if (PACKETTYPE::PT_BallPlaceRecv == t)		return "PT_BallPlaceRecv";
	else if (PACKETTYPE::PT_ConnectCheck == t)		return "PT_ConnectCheck";
	else if (PACKETTYPE::PT_Disconnect == t)		return "PT_Disconnect";
	else if (PACKETTYPE::PT_ClubSetting == t)		return "PT_ClubSetting";
	else if (PACKETTYPE::PT_ClubSettingRecv == t)	return "PT_ClubSettingRecv";
	else if (PACKETTYPE::PT_ShotData == t)			return "PT_ShotData";
	else if (PACKETTYPE::PT_ShotDataRecv == t)		return "PT_ShotDataRecv";
	else if (PACKETTYPE::PT_TeeSetting == t)		return "PT_TeeSetting";
	else if (PACKETTYPE::PT_TeeSettingRecv == t)	return "PT_TeeSettingRecv";
	else if (PACKETTYPE::PT_None == t)				return "PT_None";
	else											return "None Type Message";
}

//////////////////////////////////////////////////////////////////////////////////
// struct

typedef struct _ShotDataInfo {
	int phase;
	float ballspeed;
	float ballpitch;
	float ballyaw;
	int spinback;
	int spinside;
	float clubspeed;
	float clubpath;
	float clubface;

	//_ShotDataInfo()
	//{
	//	this->phase = 0;
	//	this->ballspeed = 0.f;
	//	this->ballpitch = 0.f;
	//	this->ballyaw = 0.f;
	//	this->spinback = 0;
	//	this->spinside = 0;
	//	this->clubspeed = 0.f;
	//	this->clubpath = 0.f;
	//	this->clubface = 0.f;
	//}

	//_ShotDataInfo(const _ShotDataInfo& sd)
	//{
	//	this->phase = sd.phase;
	//	this->ballpitch = sd.ballpitch;
	//	this->ballspeed = sd.ballspeed;
	//	this->ballyaw = sd.ballyaw;
	//	this->spinback = sd.spinback;
	//	this->spinside = sd.spinside;
	//	this->clubface = sd.clubface;
	//	this->clubpath = sd.clubpath;
	//	this->clubspeed = sd.clubspeed;
	//}
}ShotDataInfo;




/*
Packet
type : PACKETTYPE, � ��Ŷ�� �Ѿ�Դ��� ����
size : �ش� Ÿ�� ��Ŷ�� ������
*/

class Packet
{
public:
	//�Ϲ� ������
	Packet()
	{
		this->type = PACKETTYPE::PT_None;
		this->size = PACKETHEADER;
	}

	//type�� ������ ������(send����(~~recv ��Ŷ ���))
	Packet(const PACKETTYPE& type)
	{
		this->type = type;
		this->size = PACKETHEADER;
	}

	~Packet()
	{

	};

	//type ���� ��
	void SetType(const PACKETTYPE& _type)
	{
		this->type = _type;
	}

	//size ���� ��(data ���� �� ���)
	void SetSize(const unsigned int& _size)
	{
		this->size = _size;
	}

	PACKETTYPE& GetType()
	{
		return this->type;
	}
	unsigned int& GetSize()
	{
		return this->size;
	}

protected:
	PACKETTYPE type;
	unsigned int size;
};

#pragma pack(push, 1)
class PacketShotDataInfo : public Packet
{
public:

	PacketShotDataInfo()
	{
		this->type = PACKETTYPE::PT_ShotData;
		this->size = PACKETHEADER + sizeof(ShotDataInfo);
		this->data = ShotDataInfo{};
	}
	PacketShotDataInfo(const ShotDataInfo& data)
	{
		this->type = PACKETTYPE::PT_ShotData;
		this->size = PACKETHEADER + sizeof(ShotDataInfo);
		this->data = data;
	}
	~PacketShotDataInfo() = default;

	void SetData(const ShotDataInfo& _data)
	{
		this->data = _data;
	}
	ShotDataInfo& GetData()
	{
		return this->data;
	}

private:
	ShotDataInfo data;
};

class PacketBallPlace : public Packet
{
public:

	PacketBallPlace()
	{
		this->type = PACKETTYPE::PT_BallPlace;
		this->size = PACKETHEADER + sizeof(BALLPLACE);
		this->data = BALLPLACE::OUTOFBOUND;
	}
	PacketBallPlace(const BALLPLACE& data)
	{
		this->type = PACKETTYPE::PT_BallPlace;
		this->size = PACKETHEADER + sizeof(BALLPLACE);
		this->data = data;
	}
	~PacketBallPlace() = default;

	void SetData(const BALLPLACE& _data)
	{
		this->data = _data;
	}
	BALLPLACE& GetData()
	{
		return this->data;
	}

private:
	BALLPLACE data;
};

class PacketActiveState : public Packet
{
public:

	PacketActiveState()
	{
		this->type = PACKETTYPE::PT_ActiveState;
		this->size = PACKETHEADER + sizeof(bool);
		this->data = false;
	}
	PacketActiveState(const bool& data)
	{
		this->type = PACKETTYPE::PT_ActiveState;
		this->size = PACKETHEADER + sizeof(bool);
		this->data = data;
	}
	~PacketActiveState() = default;

	void SetData(const bool& _data)
	{
		this->data = _data;
	}
	bool& GetData()
	{
		return this->data;
	}

private:
	bool data;
};

class PacketTeeSetting : public Packet
{
public:

	PacketTeeSetting()
	{
		this->type = PACKETTYPE::PT_TeeSetting;
		this->size = PACKETHEADER + sizeof(TEESETTING);
		this->data = TEESETTING::T40;
	}
	PacketTeeSetting(const TEESETTING& data)
	{
		this->type = PACKETTYPE::PT_TeeSetting;
		this->size = PACKETHEADER + sizeof(TEESETTING);
		this->data = data;
	}
	~PacketTeeSetting() = default;

	void SetData(const TEESETTING& _data)
	{
		this->data = _data;
	}
	TEESETTING& GetData()
	{
		return this->data;
	}

private:
	TEESETTING data;
};

class PacketClubSetting : public Packet
{
public:

	PacketClubSetting()
	{
		this->type = PACKETTYPE::PT_ClubSetting;
		this->size = PACKETHEADER + sizeof(CLUBSETTING);
		this->data = CLUBSETTING::DRIVER;
	}
	PacketClubSetting(const CLUBSETTING& data)
	{
		this->type = PACKETTYPE::PT_ClubSetting;
		this->size = PACKETHEADER + sizeof(CLUBSETTING);
		this->data = data;
	}
	~PacketClubSetting() = default;

	void SetData(const CLUBSETTING& _data)
	{
		this->data = _data;
	}
	CLUBSETTING& GetData()
	{
		return this->data;
	}

private:
	CLUBSETTING data;
};
#pragma pack(pop)