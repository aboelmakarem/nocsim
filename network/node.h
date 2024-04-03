/* 	node.h
	Ahmed Hussein (amhussein4@gmail.com)
	04/03/2024

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

#ifndef NODE_H_
#define NODE_H_

#include "address.h"
#include "channel.h"

namespace network
{
	enum class NodeType
	{
		Terminal,
		Router
	};

	class Node
	{
	public:
		Node(Address address);
		Node(const Node& node) = delete;
		Node(Node&& node) = delete;
		virtual ~Node();
		Node& operator=(const Node& node) = delete;
		Node& operator=(Node&& node) = delete;
		const Address& address() const;
		virtual NodeType type() const = 0;
		virtual void addInChannel(Channel* channel) = 0;
		virtual void addOutChannel(Channel* channel) = 0;
		virtual void update(unsigned timestep) = 0;

	protected:
		Address address_;
	};
}

#endif
