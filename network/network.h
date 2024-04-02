// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 1st 2024

#ifndef NETWORK_H_
#define NETWORK_H_

#include "channel.h"
#include "router.h"
#include <vector>

namespace network
{
	class Network
	{
	public:
		Network();
		Network(const Network& network) = delete;
		Network(Network&& network) = delete;
		virtual ~Network();
		Network& operator=(const Network& network) = delete;
		Network& operator=(Network&& network) = delete;
		virtual void build(unsigned channelWidth,unsigned bufferSize) = 0;
		virtual void update() = 0;

	protected:
		std::vector<Router*> routers_;
		std::vector<Channel*> channels_;
	};

	class TorusNetwork : public Network
	{
	public:
		TorusNetwork(const std::vector<unsigned>& sizes);
		TorusNetwork(const TorusNetwork& network) = delete;
		TorusNetwork(TorusNetwork&& network) = delete;
		~TorusNetwork();
		TorusNetwork& operator=(const TorusNetwork& network) = delete;
		TorusNetwork& operator=(TorusNetwork&& network) = delete;
		void build(unsigned channelWidth,unsigned bufferSize);
		void update();
		void print() const;

	private:
		void indexToAddress(unsigned index,Address& address) const;
		unsigned addressToIndex(const Address& address) const;
		unsigned dimensionality_{1};
		std::vector<unsigned> sizes_;
		unsigned routerCount_{0};
		unsigned channelCount_{0};
	};
}

#endif
