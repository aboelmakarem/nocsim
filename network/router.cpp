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

namespace network
{
	Router::Router(Address address) : Node(address) {}
	Router::~Router() {}
	NodeType Router::type() const
	{
		return NodeType::Router;
	}

	TorusRouter::TorusRouter(Address address) : Router(address)
	{
		// allocate channel vectors
		outPositiveChannels_.resize(address_.size(),nullptr);
		inPositiveChannels_.resize(address_.size(),nullptr);
		outNegativeChannels_.resize(address_.size(),nullptr);
		inNegativeChannels_.resize(address_.size(),nullptr);
	}
	TorusRouter::~TorusRouter() {}
	void TorusRouter::print() const
	{
		printf("router @ ");
		printAddress(address_);
		printf("\n\tin channels (%lu)\n",inPositiveChannels_.size() + inNegativeChannels_.size());
		for(auto channel : inPositiveChannels_)
		{
			printf("\t");
			channel->print();
		}
		for(auto channel : inNegativeChannels_)
		{
			printf("\t");
			channel->print();
		}
		printf("\n\tout channels (%lu)\n",outPositiveChannels_.size() + outNegativeChannels_.size());
		for(auto channel : outPositiveChannels_)
		{
			printf("\t");
			channel->print();
		}
		for(auto channel : outNegativeChannels_)
		{
			printf("\t");
			channel->print();
		}
	}
	void TorusRouter::addInChannel(Channel* channel)
	{
		assert(channel != nullptr);
		assert(address_ == channel->end()->address());
		Address source = channel->start()->address();
		if(address_ == source)
		{
			// this is the injection channel
			assert(inTerminal_ == nullptr);
			inTerminal_ = channel;
			return;
		}
		int adjacency = addressAdjacency(address_,source);
		assert(adjacency != 0);
		if(adjacency > 0)
		{
			// source is ahead of this node
			inPositiveChannels_[adjacency - 1] = channel;
		}
		else
		{
			// this node is ahead of source
			inNegativeChannels_[-adjacency - 1] = channel;
		}
	}
	void TorusRouter::addOutChannel(Channel* channel)
	{
		assert(channel != nullptr);
		assert(address_ == channel->start()->address());
		Address destination = channel->end()->address();
		if(address_ == destination)
		{
			// this is the ejection channel
			assert(outTerminal_ == nullptr);
			outTerminal_ = channel;
			return;
		}
		int adjacency = addressAdjacency(address_,destination);
		assert(adjacency != 0);
		if(adjacency > 0)
		{
			// destination is ahead of source
			outPositiveChannels_[adjacency - 1] = channel;
		}
		else
		{
			// this node is ahead of destination
			outNegativeChannels_[-adjacency - 1] = channel;
		}
	}
	void TorusRouter::update(unsigned time)
	{
		// Go over all in channels and pick a packet 
		// that is ready to be routed. If more than 
		// one packet is ready, pick the one with the 
		// least Manhattan distance to its destination.
		Packet* packet = nullptr;
		unsigned distance = 0;
		unsigned minDistance = UINT_MAX;
		Packet* minDistancePacket = nullptr;
		// start with injected packets
		packet = inTerminal_->frontPacket();
		if(packet != nullptr)
		{
			distance = manhattanDistance(address_,packet->destination());
			if(distance < minDistance)
			{
				minDistance = distance;
				minDistancePacket = packet;
			}
		}
		// then check received packets from both directions
		for(auto channel : inPositiveChannels_)
		{
			packet = channel->frontPacket();
			if(packet != nullptr)
			{
				distance = manhattanDistance(address_,packet->destination());
				if(distance < minDistance)
				{
					minDistance = distance;
					minDistancePacket = packet;
				}
			}
		}
		for(auto channel : inNegativeChannels_)
		{
			packet = channel->frontPacket();
			if(packet != nullptr)
			{
				distance = manhattanDistance(address_,packet->destination());
				if(distance < minDistance)
				{
					minDistance = distance;
					minDistancePacket = packet;
				}
			}
		}
		if(minDistancePacket == nullptr)
		{
			return;
		}
		printf("picked packet %u\n",minDistancePacket->id());
		printf("\t\t heading to");fflush(0);
		printAddress(minDistancePacket->destination());
		route(minDistancePacket);
		printf("done routing\n");fflush(0);
	}
	void TorusRouter::send(Packet* packet) {}
	void TorusRouter::route(Packet* packet) {}
	Channel* TorusRouter::pickRoutingChannel(const Packet* packet) const
	{
		// Routing Algorithm:
		// Given a packet, pick the output channel that reduces 
		// the difference between the address of this router and 
		// destination router along some dimension. 
		// 1. Pick the dimension with the greatest difference. 
		// This is a greedy mininal step. 
		// 2. If the output channel in that direction is congested, 
		// pick the one with the second largest difference, then
		// the third, and so on until an uncongested channel is
		// found. This is an adaptive step. 
		// 3. If all non-zero difference dimensions are congested, 
		// pick a random, uncongested, zero-difference dimension 
		// channel. This is a misrouting step. 
		// 4. If all channels are congested, do not route the packet.
		const Address& destination = packet->destination();
		unsigned dimensionality = address_.size();
		Address difference(dimensionality,0);
		Address permutation(dimensionality,0);
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			difference[i] = destination[i] - address_[i];
			permutation[i] = i;
		}
		// sort the differences ascendingly
		auto diffcomparator = [difference](unsigned i,unsigned j)
		{
			return (difference[i] < difference[j]);
		};
		std::sort(permutation.begin(),permutation.end(),diffcomparator);
		// route according to the above algorithm description
		Channel* channel = nullptr;
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			int dimension = permutation[dimensionality - i - 1];
			if(difference[dimension] > 0)
			{
				// pick from channels outgoing in the positive direction
				channel = outPositiveChannels_[dimension];
			}
			else if(difference[dimension] < 0)
			{
				// pick from channels outgoing in the negative direction
				channel = outNegativeChannels_[dimension];
			}
			// difference is zero, pick the less loaded direction to 
			// misroute
			if(outPositiveChannels_[dimension]->pushLoad() > outNegativeChannels_[dimension]->pushLoad())
			{
				channel = outNegativeChannels_[dimension];
			}
			else
			{
				channel = outPositiveChannels_[dimension];
			}
			if(channel->pushable())
			{
				return channel;
			}
		}
		return nullptr;
	}
}

