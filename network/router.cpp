// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 1st 2024

#include "router.h"
#include "stdio.h"
#include "limits.h"
#include <cassert>

namespace {
	void printUintVector(const std::vector<unsigned>& vectorToPrint)
	{
		printf(" ");
		for(auto index : vectorToPrint)
		{
			printf("%u,",index);
		}
	}
	unsigned manhattanDistance(const network::Address& address1,const network::Address& address2)
	{
		unsigned dimensionality = address1.size();
		if(address2.size() != dimensionality)
		{
			return 0;
		}
		unsigned distance = 0;
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			distance += (address1[i] > address2[i]) ? (address1[i] - address2[i]) : (address2[i] - address1[i]);
		}
		return distance;
	}
}

namespace network
{
	Router::Router(Address address) : address_(address) {}
	Router::~Router() {}
	void Router::addInChannel(Channel* channel)
	{
		assert(channel != nullptr);
		inChannels_.push_back(channel);
	}
	void Router::addOutChannel(Channel* channel)
	{
		assert(channel != nullptr);
		outChannels_.push_back(channel);
	}
	const std::vector<unsigned>& Router::address() const
	{
		return address_;
	}
	void Router::inject(Packet* packet)
	{

	}
	void Router::update(unsigned time)
	{
		// Go over all in channels and pick a packet 
		// that is ready to be transmitted. If more than 
		// one packet is ready, pick the one with the 
		// least Manhattan distance to its destination.
		Packet* packet = nullptr;
		unsigned distance = 0;
		unsigned minDistance = UINT_MAX;
		Packet* minDistancePacket = nullptr;
		for(auto channel : inChannels_)
		{
			packet = channel->frontPacket();
			if(packet != nullptr)
			{
				distance = manhattanDistance(address_,packet->destination()->address_);
				if(distance < minDistance)
				{
					minDistance = distance;
					minDistancePacket = packet;
				}
			}
		}
		
	}
	void Router::print() const
	{
		printf("router @ ");
		printAddress();
		printf("\n\tin channels (%lu)\n",inChannels_.size());
		for(auto channel : inChannels_)
		{
			printf("\t");
			channel->print();
		}
		printf("\n\tout channels (%lu)\n",inChannels_.size());
		for(auto channel : outChannels_)
		{
			printf("\t");
			channel->print();
		}
	}
	void Router::printAddress() const
	{
		printUintVector(address_);
	}
}

