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

namespace network
{
	Network::Network() {}
	Network::~Network()
	{
		for(auto router : routers_)
		{
			delete router;
		}
		for(auto channel : channels_)
		{
			delete channel;
		}
	}
	void Network::runRandomTrafficLoad(unsigned totalTime,float loadFactor)
	{
		unsigned timestep = 0;
		while(timestep < totalTime)
		{
			loadFactor
			for(auto router : routers_)
			{
				router->update(timestep);
			}
			++timestep;
		}
	}

	TorusNetwork::TorusNetwork(const std::vector<unsigned>& sizes) : dimensionality_(sizes.size())
	{
		sizes_.resize(dimensionality_);
		routerCount_ = 1;
		for(unsigned i = 0 ; i < dimensionality_ ; ++i)
		{
			sizes_[i] = sizes[i];
			routerCount_ *= sizes_[i];
		}
		// For torus networks, each node has d incoming channels and 
		// d outgoing channels where d is the dimensionality of the 
		// network. Add 2 channels per router for injection and 
		// ejection.
		channelCount_ = 2*(dimensionality_ + 1)*routerCount_;
	}
	TorusNetwork::~TorusNetwork() {}
	void TorusNetwork::build(unsigned channelWidth,unsigned bufferSize)
	{
		// delete any existing routers or channels
		for(auto channel : channels_)
		{
			delete channel;
		}
		for(auto router : routers_)
		{
			delete router;
		}
		// allocate router and channel arrays
		routers_.resize(routerCount_);
		channels_.resize(channelCount_);
		// create routers
		Address address;
		address.resize(dimensionality_,0);
		Channel* channel = nullptr;
		unsigned channelIndex = 0;
		for(unsigned i = 0 ; i < routerCount_ ; ++i)
		{
			indexToAddress(i,address);
			routers_[i] = new TorusRouter(address);
			// Create an injection channel and an ejection 
			// channel for each router. These channels must 
			// be the first channels in the list of channels 
			// held by the routers. 
			channel = new Channel(channelWidth,bufferSize,nullptr,routers_[i]);
			routers_[i]->addInChannel(channel);
			channels_[channelIndex++] = channel;
			channel = new Channel(channelWidth,bufferSize,routers_[i],nullptr);
			routers_[i]->addOutChannel(channel);
			channels_[channelIndex++] = channel;
		}
		// create channels, go over all routers and create 
		// outgoing channels only, incoming channels for a 
		// router are created as outgoing channels of adjacent 
		// routers.  
		Address sourceAddress(dimensionality_,0);
		Address targetAddress(dimensionality_,0);
		Router* source = nullptr;
		Router* target = nullptr;
		for(unsigned i = 0 ; i < routerCount_ ; ++i)
		{
			indexToAddress(i,sourceAddress);
			indexToAddress(i,targetAddress);
			source = routers_[i];
			for(unsigned dim = 0 ; dim < dimensionality_ ; ++dim)
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
		for(auto router : routers_)
		{
			router->print();
		}
	}
	void TorusNetwork::indexToAddress(unsigned index,Address& address) const
	{
		unsigned blockSize = routerCount_;
		for(int i = 0 ; i < dimensionality_ ; ++i)
		{
			blockSize /= sizes_[i];
			address[i] = index/blockSize;
			index -= address[i]*blockSize;
		}
	}
	unsigned TorusNetwork::addressToIndex(const Address& address) const
	{
		unsigned blockSize = 1;
		unsigned index = address[dimensionality_ - 1];
		for(int i = dimensionality_ - 2 ; i >= 0 ; --i)
		{
			blockSize *= sizes_[i + 1];
			index += address[i]*blockSize;
		}
		return index;
	}
}

