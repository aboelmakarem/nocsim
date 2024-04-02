// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 2nd 2024

#include "channel.h"
#include "router.h"
#include <stdio.h>

namespace network
{
	Channel::Channel(unsigned width,unsigned bufferSize,Router* start,Router* end) : width_(width), bufferSize_(bufferSize), start_(start), end_(end) {}
	Channel::~Channel() {}
	void Channel::print() const
	{
		printf("channel ");
		start_->printAddress();
		printf(" --> ");
		end_->printAddress();
		printf("\n");
	}
	bool Channel::push(Packet* packet)
	{
		// do not push packets if in buffer is full
		if(inBuffer_.size() >= bufferSize_)
		{
			return false;
		}
		inBuffer_.push_back(packet);
		return true;
	}
	bool Channel::transmit()
	{
		// move a packet from in buffer to out buffer if possible
		if(inBuffer_.empty())
		{
			return false;
		}
		if(outBuffer_.size() >= bufferSize_)
		{
			return false;
		}
		outBuffer_.push_back(inBuffer_.front());
		inBuffer_.pop_front();
		return true;
	}
	Packet* Channel::pop()
	{
		if(outBuffer_.empty())
		{
			return nullptr;
		}
		Packet* packet = outBuffer_.front();
		outBuffer_.pop_front();
		return packet;
	}
	Packet* Channel::frontPacket() const
	{
		if(outBuffer_.empty())
		{
			return nullptr;
		}
		return outBuffer_.front();
	}
}

