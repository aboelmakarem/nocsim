// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 2nd 2024

#include "packet.h"
#include "router.h"

namespace network
{
	Packet::Packet(unsigned size,Router* destination) : size_(size), destination_(destination) {}
	Packet::~Packet() {}
	const Router* Packet::destination() const
	{
		return destination_;
	}
}

