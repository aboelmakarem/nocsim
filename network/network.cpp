// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 1st 2024

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
		// network. 
		channelCount_ = 2*dimensionality_*routerCount_;
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
		for(unsigned i = 0 ; i < routerCount_ ; ++i)
		{
			indexToAddress(i,address);
			routers_[i] = new Router(address);
		}
		// create channels, go over all routers and create 
		// outgoing channels only, incoming channels for a 
		// router are created as outgoing channels of adjacent 
		// routers.  
		Address sourceAddress(dimensionality_,0);
		Address targetAddress(dimensionality_,0);
		Router* source = nullptr;
		Router* target = nullptr;
		Channel* channel = nullptr;
		unsigned index = 0;
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
				channels_[index++] = channel;
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
				channels_[index++] = channel;
				// restore dimension
				targetAddress[dim] = sourceAddress[dim];
			}
		}
	}
	void TorusNetwork::update() {}
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

