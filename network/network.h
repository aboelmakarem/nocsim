/* 	network.h
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
		virtual void update(unsigned timestep) = 0;
		void runRandomTrafficLoad(unsigned totalTime,float loadFactor);

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
		void update(unsigned timestep);
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
