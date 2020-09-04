
#pragma once
#include <array>

class Packet
{

public:
	Packet() = default;
	Packet(const unsigned char * data, unsigned int dataCount, unsigned int packetId)
		: m_rawData(std::make_unique<std::byte[]>(dataCount))
		, m_length(dataCount)
		, m_packetId(packetId)
	{
		
		std::memcpy(m_rawData.get(), data, dataCount);
	}

	size_t GetPacketId() const { return m_packetId;  }
	void Сlear() { m_rawData.release(); m_packetId = 0; m_length = 0; }
	size_t GetSizeData() const { return m_length; }
	std::unique_ptr<std::byte[]> & GetRawData() { return m_rawData; }
private:
	std::unique_ptr<std::byte[]>	m_rawData;
	size_t							m_length{ 0 };
	size_t							m_packetId{ 0 };
};





template<size_t MaxPacketLength, size_t MaxPacketNumber, size_t TotalBytesLength>
class PacketQueue
{
	static_assert(MaxPacketLength > 0,
		"Invalid packet queue configuration: max length of packet must be at least one byte!");
	static_assert(MaxPacketNumber > 0,
		"Invalid packet queue configuration: max number of packets must be at least one byte!");
public:
	PacketQueue() = default;
	PacketQueue(const PacketQueue &) = delete;
	PacketQueue& operator=(const PacketQueue &) = delete;


	bool IsEmpty() const noexcept;

	bool HasId(unsigned int id) const noexcept;

	void Clear();


		/**
	 * \brief Поместить пакет в очередь
	 * \param data содержимое пакета
	 * \param dataCount длина пакета в байтах
	 * \param packetId идентификатор пакета
	 * \return результат постановки пакета в очередь:
	 *	true - пакет помещен в очередь
	 *	false - длина пакета превышает максимально допустимую, число пакетов в очереди или суммарный размер пакетов в очереди превысили максимально допустимые*/
	bool Push(unsigned char *data, unsigned int dataCount, unsigned int packetId);

	/**
	 * \brief Извлечь пакет из очереди
	 * \param buffer buffer - приемный буфер, куда помещаются данные извлеченные из очереди
	 * \param bufferSize bufferSize - размер приемного буфера
	 * \param dataCount dataCount - фактический размер пакета, извлеченного из очереди
	 * \param packetId packetId - идентификатор пакета, извлеченного из очереди
	 * \return return - результат извлечения пакета из очереди:
	 *	true - пакет извлечен и помещен в буфер (в том числе, если размер пакета превысил размер приемного буфера)
	 *	false - очередь пуста
	 */
	bool Pop(unsigned char *buffer, unsigned int bufferSize, unsigned int &dataCount,
		unsigned int &packetId);
private:
	size_t									  m_currentCountPacket {0};
	size_t									  m_begin {0};
	size_t									  m_end {0};
	std::array <Packet, MaxPacketNumber>	  m_dataPacket;
};

#include "PacketQueue.inl"