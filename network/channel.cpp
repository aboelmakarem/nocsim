/* 	channel.cpp
	Ahmed Hussein (amhussein4@gmail.com)
	04/02/2024

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

#include "channel.h"
#include "node.h"
#include <stdio.h>
#include <cassert>

namespace network
{
	Channel::Channel(unsigned width,unsigned bufferSize,Node* start,Node* end) : width_(width), bufferSize_(bufferSize), start_(start), end_(end)
	{
		assert(start_ != nullptr);
		assert(end_ != nullptr);
	}
	Channel::~Channel() {}
	void Channel::print() const
	{
		printf("channel ");
		printAddress(start_->address());
		printf(" --> ");
		printAddress(end_->address());
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
	unsigned Channel::width() const
	{
		return width_;
	}
	unsigned Channel::bufferSize() const
	{
		return bufferSize_;
	}
	Node* Channel::start() const
	{
		return start_;
	}
	Node* Channel::end() const
	{
		return end_;
	}
	bool Channel::pushable() const
	{
		return (inBuffer_.size() < bufferSize_);
	}
	bool Channel::poppable() const
	{
		return (outBuffer_.size() > 0);
	}
	unsigned Channel::pushLoad() const
	{
		return outBuffer_.size();
	}
}

