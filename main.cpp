#include <iostream>
#include <numeric>
#include <vector>
#include "PacketQueue.h"

int main()
{
	{
		PacketQueue<255, 10, 200> packetQueue;

		auto data = std::vector<unsigned char>(1000);
		std::iota(data.begin(), data.end(), 0);

		std::cout << "MaxPacketNumber checking..." << std::endl;
		for (int i = 0; i < 100; ++i)
		{
			bool r = packetQueue.Push(data.data(), 10, 0);
			if (i < 10 && !r ||
				i >= 10 && r)
			{
				std::cerr << "Failed on " << i << std::endl;
			}
		}

		for (int i = 0; i < 10; ++i)
		{
			bool r = packetQueue.Push(data.data(), 10, 0);
			if (i < 10 && !r ||
				i >= 10 && r)
			{
				std::cerr << "Failed on " << i << std::endl;
			}
		}
	}

	PacketQueue<10, 10, 200> packetQueue;

	auto data = std::vector<unsigned char>(1000);
	std::iota(data.begin(), data.end(), unsigned char(0));

	std::array<unsigned char, 1000> buffer;
	buffer.fill(42);

	unsigned dataCount = 0, packetId = 0;
	bool result = false;

	// must do nothing and return false
	result = packetQueue.Pop(buffer.data(), static_cast<unsigned>(buffer.size()), dataCount, packetId);
	if (result || buffer[0] != 42 || dataCount != 0 || packetId != 0)
	{
		return 1;
	}

	// must do nothing and return true
	packetQueue.Push(data.data(), 0, 555);
	result = packetQueue.Pop(buffer.data(), static_cast<unsigned>(buffer.size()), dataCount, packetId);
	if (!result || buffer[0] != 42 || dataCount != 0 || packetId != 555)
	{
		return 1;
	}

	// must set buffer[0] to zero
	packetQueue.Push(data.data(), 1, 666);
	result = packetQueue.Pop(buffer.data(), static_cast<unsigned>(buffer.size()), dataCount, packetId);
	if (!result || buffer[0] != 0 || dataCount != 1 || packetId != 666)
	{
		return 1;
	}


	return 0;
}