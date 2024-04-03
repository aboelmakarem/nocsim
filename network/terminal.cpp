/* 	terminal.cpp
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

#include "terminal.h"
#include "stdio.h"
#include <cassert>

namespace network
{
	Terminal::Terminal(Address address) : Node(address) {}
	Terminal::~Terminal() {}
	NodeType Terminal::type() const
	{
		return NodeType::Terminal;
	}
	void Terminal::addInChannel(Channel* channel)
	{
		assert(channel != nullptr);
		assert(address_ == channel->end()->address());
		assert(inChannel_ == nullptr);
		inChannel_ = channel;
	}
	void Terminal::addOutChannel(Channel* channel)
	{
		assert(channel != nullptr);
		assert(address_ == channel->start()->address());
		assert(outChannel_ == nullptr);
		outChannel_ = channel;
	}
	void Terminal::update(unsigned time)
	{
		// eject packets from in channel if there are that are 
		// ready
		if(!inChannel_->poppable())
		{
			return;
		}
		Packet* packet = inChannel_->pop();
		printf("terminal ");
		printAddress(address_);
		printf(" received packet %u\n",packet->id());
		delete packet;
	}
	void Terminal::print() const
	{
		printf("terminal @ ");
		printAddress(address_);
		printf("\n\tin channel:\t");
		inChannel_->print();
		printf("\n\tout channel:\t");
		outChannel_->print();
	}
}

