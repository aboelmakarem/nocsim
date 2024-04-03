/* 	network.cpp
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

#include "network.h"
#include "random.h"
#include "stdio.h"

namespace network
{
	Network::Network() {}
	Network::~Network()
	{
		collapse();
	}
	void Network::runRandomTrafficLoad(unsigned totalTime,float loadFactor)
	{
		/*unsigned timestep = 0;
		unsigned packetID = 0;
		while(timestep < totalTime)
		{
			if(rand_uniform() < loadFactor)
			{
				Router* source = routers_[rand_int(0,routers_.size())];
				Router* destination = routers_[rand_int(0,routers_.size())];
				if(!source->inject(new Packet(packetID++,100,destination->address())))
				{
					printf("failed to inject\n");
				}
			}
			for(auto channel : channels_)
			{
				channel->transmit();
			}
			for(auto router : routers_)
			{
				router->update(timestep);
			}
			++timestep;
		}*/
	}
	void Network::collapse()
	{
		for(auto terminal : terminals_)
		{
			delete terminal;
		}
		for(auto router : routers_)
		{
			delete router;
		}
		for(auto channel : channels_)
		{
			delete channel;
		}
	}

	TorusNetwork::TorusNetwork(const std::vector<unsigned>& sizes) : sizes_(sizes) {}
	TorusNetwork::~TorusNetwork() {}
	void TorusNetwork::build(unsigned channelWidth,unsigned bufferSize)
	{
		// delete any existing routers or channels
		collapse();
		// allocate terminal, router and channel arrays
		unsigned dimensionality = sizes_.size();
		unsigned routerCount = 1;
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			routerCount *= sizes_[i];
		}
		routers_.resize(routerCount);
		terminals_.resize(routerCount);
		// For torus networks, each router has d incoming channels and 
		// d outgoing channels where d is the dimensionality of the 
		// network. Add 2 channels per router for injection and 
		// ejection.
		channels_.resize(2*(dimensionality + 1)*routerCount);
		// create terminals and routers
		Address address;
		address.resize(dimensionality,0);
		Channel* channel = nullptr;
		unsigned channelIndex = 0;
		for(unsigned i = 0 ; i < routerCount ; ++i)
		{
			indexToAddress(i,address);
			terminals_[i] = new Terminal(address);
			routers_[i] = new TorusRouter(address);
			// Create an injection channel and an ejection 
			// channel for each router. 
			channel = new Channel(channelWidth,bufferSize,terminals_[i],routers_[i]);
			terminals_[i]->addOutChannel(channel);
			routers_[i]->addInChannel(channel);
			channels_[channelIndex++] = channel;
			channel = new Channel(channelWidth,bufferSize,routers_[i],terminals_[i]);
			routers_[i]->addOutChannel(channel);
			terminals_[i]->addInChannel(channel);
			channels_[channelIndex++] = channel;
		}
		// create channels, go over all routers and create 
		// outgoing channels only, incoming channels for a 
		// router are created as outgoing channels of adjacent 
		// routers.  
		Address sourceAddress(dimensionality,0);
		Address targetAddress(dimensionality,0);
		Router* source = nullptr;
		Router* target = nullptr;
		for(unsigned i = 0 ; i < routerCount ; ++i)
		{
			indexToAddress(i,sourceAddress);
			indexToAddress(i,targetAddress);
			source = routers_[i];
			for(unsigned dim = 0 ; dim < dimensionality ; ++dim)
			{
				// increment dimension
				targetAddress[dim] = (sourceAddress[dim] + 1)%sizes_[dim];
				// create outgoing channel
				target = routers_[addressToIndex(targetAddress)];
				channel = new Channel(channelWidth,bufferSize,source,target);
				source->addOutChannel(channel);
				target->addInChannel(channel);
				channels_[channelIndex++] = channel;
				// decrement dimension
				if(sourceAddress[dim] == 0)
				{
					targetAddress[dim] = sizes_[dim] - 1;
				}
				else
				{
					targetAddress[dim] = sourceAddress[dim] - 1;
				}
				// create outgoing channel
				target = routers_[addressToIndex(targetAddress)];
				channel = new Channel(channelWidth,bufferSize,source,target);
				source->addOutChannel(channel);
				target->addInChannel(channel);
				channels_[channelIndex++] = channel;
				// restore dimension
				targetAddress[dim] = sourceAddress[dim];
			}
		}
	}
	void TorusNetwork::update(unsigned timestep) {}
	void TorusNetwork::print() const
	{
		for(auto terminal : terminals_)
		{
			terminal->print();
		}
		for(auto router : routers_)
		{
			router->print();
		}
	}
	void TorusNetwork::indexToAddress(unsigned index,Address& address) const
	{
		unsigned dimensionality = sizes_.size();
		unsigned blockSize = routers_.size();
		for(int i = 0 ; i < dimensionality ; ++i)
		{
			blockSize /= sizes_[i];
			address[i] = index/blockSize;
			index -= address[i]*blockSize;
		}
	}
	unsigned TorusNetwork::addressToIndex(const Address& address) const
	{
		unsigned dimensionality = sizes_.size();
		unsigned blockSize = 1;
		unsigned index = address[dimensionality - 1];
		for(int i = dimensionality - 2 ; i >= 0 ; --i)
		{
			blockSize *= sizes_[i + 1];
			index += address[i]*blockSize;
		}
		return index;
	}
}

