#pragma once

template <size_t MaxPacketLength, size_t MaxPacketNumber, size_t TotalBytesLength>
bool PacketQueue<MaxPacketLength, MaxPacketNumber, TotalBytesLength>::Push(unsigned char* data, unsigned dataCount, unsigned packetId)
{
	if (nullptr == data || m_currentCountPacket >= MaxPacketNumber || dataCount > MaxPacketLength)
	{
		return false;
	}

	int freeSpace = TotalBytesLength;
	for(auto && packet : m_dataPacket)
	{
		freeSpace -= packet.GetSizeData();
	}

	if(freeSpace > dataCount )
	{

		m_dataPacket[m_end] = Packet(data, dataCount, packetId);


		if(m_end + 1 == m_dataPacket.size() && m_begin != 0)
		{
			m_end = 0;
		}
		else
		{
			m_end++;
		}

		m_currentCountPacket++;

		return true;
	}
	return false;
}

template <size_t MaxPacketLength, size_t MaxPacketNumber, size_t TotalBytesLength>
bool PacketQueue<MaxPacketLength, MaxPacketNumber, TotalBytesLength>::Pop(unsigned char* buffer, unsigned bufferSize, unsigned& dataCount, unsigned& packetId)
{
	if(IsEmpty() || buffer == nullptr)
	{
		return false;
	}

	auto && packet = std::exchange(m_dataPacket[m_begin], Packet());
	dataCount = std::min(bufferSize, packet.GetSizeData());
	std::memcpy(buffer, packet.GetRawData().get(), dataCount);

	packetId = packet.GetPacketId();
	m_dataPacket[m_begin].Сlear();

	if(m_begin + 1 == m_dataPacket.size())
	{
		m_begin = 0;
	}
	else
	{
		m_begin++;
	}
	
	m_currentCountPacket--;
	return true;
}

template <size_t MaxPacketLength, size_t MaxPacketNumber, size_t TotalBytesLength>
void PacketQueue<MaxPacketLength, MaxPacketNumber, TotalBytesLength>::Clear()  
{
	for (auto && paket : m_dataPacket)
	{
		paket.Сlear();
	}
	m_currentCountPacket = 0;
	m_begin = 0;
	m_end = 0;
}

template <size_t MaxPacketLength, size_t MaxPacketNumber, size_t TotalBytesLength>
bool PacketQueue<MaxPacketLength, MaxPacketNumber, TotalBytesLength>::HasId(unsigned id) const noexcept
{
	for(auto && paket : m_dataPacket)
	{
		if(paket.GetPacketId() == id)
		{
			return true;
		}
	}
	return false;
}

template <size_t MaxPacketLength, size_t MaxPacketNumber, size_t TotalBytesLength>
bool PacketQueue<MaxPacketLength, MaxPacketNumber, TotalBytesLength>::IsEmpty() const noexcept
{
	return m_currentCountPacket == 0;
}
