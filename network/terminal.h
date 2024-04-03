/* 	terminal.h
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

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "node.h"
#include "channel.h"

namespace network
{
	class Terminal : public Node
	{
	public:
		Terminal(Address address);
		Terminal(const Terminal& terminal) = delete;
		Terminal(Terminal&& terminal) = delete;
		~Terminal();
		Terminal& operator=(const Terminal& terminal) = delete;
		Terminal& operator=(Terminal&& terminal) = delete;
		NodeType type() const;
		void addInChannel(Channel* channel);
		void addOutChannel(Channel* channel);
		void update(unsigned time);
		void print() const;
		
	private:
		Channel* inChannel_{nullptr};
		Channel* outChannel_{nullptr};
	};
}

#endif
