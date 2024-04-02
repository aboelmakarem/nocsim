// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 1st 2024

#ifndef ROUTER_H_
#define ROUTER_H_

#include "packet.h"
#include "channel.h"
#include <vector>
#include <list>

namespace network
{
	typedef std::vector<unsigned> Address;

	class Router
	{
	public:
		Router(Address address);
		Router(const Router& router) = delete;
		Router(Router&& router) = delete;
		~Router();
		Router& operator=(const Router& router) = delete;
		Router& operator=(Router&& router) = delete;
		void addInChannel(Channel* channel);
		void addOutChannel(Channel* channel);
		const std::vector<unsigned>& address() const;
		void inject(Packet* packet);
		void update(unsigned time);
		void print() const;
		void printAddress() const;

	private:
		void send(Packet* packet);
		void receive(Packet* packet);
		void eject(Packet* packet);
		Address address_;
		std::list<Channel*> inChannels_;
		std::list<Channel*> outChannels_;
		unsigned bufferSize_{0};
		std::vector<Packet*> buffers_;
	};
}

#endif
