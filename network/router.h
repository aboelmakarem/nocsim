/* 	router.h
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

#ifndef ROUTER_H_
#define ROUTER_H_

#include "node.h"
#include "packet.h"
#include "channel.h"
#include <vector>
#include <list>

namespace network
{
	class Router : public Node
	{
	public:
		Router(Address address);
		Router(const Router& router) = delete;
		Router(Router&& router) = delete;
		virtual ~Router();
		Router& operator=(const Router& router) = delete;
		Router& operator=(Router&& router) = delete;
		NodeType type() const;
		virtual void print() const = 0;
		
	protected:
		virtual void send(Packet* packet) = 0;
		virtual void route(Packet* packet) = 0;
	};

	class TorusRouter : public Router
	{
	public:
		TorusRouter(Address address);
		TorusRouter(const TorusRouter& router) = delete;
		TorusRouter(TorusRouter&& router) = delete;
		~TorusRouter();
		TorusRouter& operator=(const TorusRouter& router) = delete;
		TorusRouter& operator=(TorusRouter&& router) = delete;
		void print() const;
		void addInChannel(Channel* channel);
		void addOutChannel(Channel* channel);
		void update(unsigned time);

	private:
		void send(Packet* packet);
		void route(Packet* packet);
		Channel* pickRoutingChannel(const Packet* packet) const;
		Channel* outTerminal_{nullptr};
		Channel* inTerminal_{nullptr};
		std::vector<Channel*> outPositiveChannels_;
		std::vector<Channel*> inPositiveChannels_;
		std::vector<Channel*> outNegativeChannels_;
		std::vector<Channel*> inNegativeChannels_;
	};
}

#endif
