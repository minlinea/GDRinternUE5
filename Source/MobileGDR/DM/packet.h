#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
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
	if (true == t)
	{
		return "Ready";	//준비, 가능은 에러가 남
	}
	else
	{
		return "Wait";	//대기 가능
	}
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
	FAIRWAY = 0,
	ROUGH,
	BUNKER,
	TEE,
	GREEN,
	WAIT,
	SLEEP,
	OUTOFBOUND 
};
inline const char* to_string(const BALLPLACE& t);
inline const char* to_string(const BALLPLACE& t)
{
	if (BALLPLACE::FAIRWAY == t)		return "Fairway";
	else if (BALLPLACE::ROUGH == t)		return "Rough";
	else if (BALLPLACE::BUNKER == t)	return "Bunker";
	else if (BALLPLACE::TEE == t)		return "Tee";
	else if (BALLPLACE::GREEN == t)		return "Green";
	else if (BALLPLACE::WAIT == t)		return "WAIT";
	else if (BALLPLACE::SLEEP == t)		return "SLEEP";
	else if (BALLPLACE::OUTOFBOUND == t)return "OB";
	return "NONE";
}
inline bool isIn(const std::vector<BALLPLACE>& arr, const BALLPLACE& target);
inline bool isIn(const std::vector<BALLPLACE>& arr, const BALLPLACE& target)
{
	return std::binary_search(arr.begin(), arr.end(), target);
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
}ShotDataInfo;




/*
Packet
type : PACKETTYPE, 어떤 패킷이 넘어왔는지 구분
size : 해당 타입 패킷의 사이즈
*/

class Packet
{
public:
	//일반 생성자
	Packet()
	{
		this->type = PACKETTYPE::PT_None;
		this->size = PACKETHEADER;
	}

	//type이 지정된 생성자(send응답(~~recv 패킷 사용))
	Packet(const PACKETTYPE& type)
	{
		this->type = type;
		this->size = PACKETHEADER;
	}

	~Packet()
	{

	};

	//type 변경 시
	void SetType(const PACKETTYPE& _type)
	{
		this->type = _type;
	}

	//size 변경 시(data 전달 시 사용)
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