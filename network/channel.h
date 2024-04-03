/* 	channel.h
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

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "packet.h"
#include <list>

namespace network
{
	class Node;

	class Channel
	{
	public:
		Channel(unsigned width,unsigned bufferSize,Node* start,Node* end);
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
		unsigned width() const;
		unsigned bufferSize() const;
		Node* start() const;
		Node* end() const;
		bool pushable() const;
		bool poppable() const;
		unsigned pushLoad() const;

	private:
		unsigned width_{0};
		Node* start_{nullptr};
		Node* end_{nullptr};
		std::list<Packet*> inBuffer_;
		std::list<Packet*> outBuffer_;
		unsigned bufferSize_{0};
	};
}

#endif
