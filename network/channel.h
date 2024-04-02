// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 2nd 2024

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "packet.h"
#include <list>

namespace network
{
	class Router;

	class Channel
	{
	public:
		Channel(unsigned width,unsigned bufferSize,Router* start,Router* end);
		Channel(const Channel& channel) = delete;
		Channel(Channel&& channel) = delete;
		~Channel();
		Channel& operator=(const Channel& channel) = delete;
		Channel& operator=(Channel&& channel) = delete;
		void print() const;
		bool push(Packet* packet);
		bool transmit();
		Packet* pop();
		Packet* frontPacket() const;

	private:
		unsigned width_{0};
		Router* start_{nullptr};
		Router* end_{nullptr};
		std::list<Packet*> inBuffer_;
		std::list<Packet*> outBuffer_;
		unsigned bufferSize_{0};
	};
}

#endif
