/* 	router.cpp
	Ahmed Hussein (amhussein4@gmail.com)
	04/01/2024

Copyright (c) 2024 Ahmed M. Hussein (amhussein4@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "router.h"
#include "stdio.h"
#include <limits.h>
#include <cassert>
#include <algorithm>

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
	bool Router::inject(Packet* packet)
	{
		// injection channel is always the first channel 
		// in the list of in channels
		return inChannels_.front()->push(packet);
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
	void Router::send(Packet* packet) {}
	void Router::receive(Packet* packet) {}
	Packet* Router::eject(Packet* packet)
	{
		// ejection channel is always the first channel 
		// in the list of out channels
		return outChannels_.front()->pop();
	}

	TorusRouter::TorusRouter(Address address) : Router(address) {}
	TorusRouter::~TorusRouter() {}
	void TorusRouter::update(unsigned time)
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
				distance = manhattanDistance(address_,packet->destination()->address());
				if(distance < minDistance)
				{
					minDistance = distance;
					minDistancePacket = packet;
				}
			}
		}
		
	}
	void TorusRouter::route(const Packet* packet) const
	{
		// Routing Algorithm:
		// Given a packet, pick the output channel that reduces 
		// the difference between the address of this router and 
		// destination router along some dimension. Pick the 
		// dimension with the greatest difference. This is a 
		// greedy mininal step. If the output channel in that 
		// direction is congested, pick the one with the second 
		// largest difference, then the third, and so on until 
		// an uncongested channel is found. This is an adaptive 
		// step. 
		// If all non-zero difference dimensions are congested, 
		// pick a random, uncongested, zero-difference dimension 
		// channel. This is a misrouting step. 
		// If all channels are congested, do not route the packet.
		const Address& destination = packet->destination()->address();
		unsigned dimensionality = address_.size();
		Address difference(dimensionality,0);
		Address permutation(dimensionality,0);
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			difference[i] = (address_[i] > destination[i]) ? (address_[i] - destination[i]) : (destination[i] - address_[i]);
			permutation[i] = i;
		}
		auto diffcomparator = [difference](unsigned i,unsigned j)
		{
			return (difference[i] < difference[j]);
		};
		sort(permutation.begin(),permutation.end(),diffcomparator);
	}
}

