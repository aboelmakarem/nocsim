/* 	address.cpp
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

#include "address.h"
#include <stdio.h>
#include <cassert>

namespace network
{
	bool operator==(const Address& address1,const Address& address2)
	{
		unsigned dimensionality = address1.size();
		assert(address2.size() == dimensionality);
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			if(address1[i] != address2[i])
			{
				return false;
			}
		}
		return true;
	}
	void printAddress(const Address& address)
	{
		printf(" %d,",address[0]);
		for(unsigned i = 1 ; i < address.size() - 1 ; ++i)
		{
			printf("%d,",address[i]);
		}
		printf("%d ",address[address.size() - 1]);
	}
	unsigned manhattanDistance(const Address& address1,const Address& address2)
	{
		unsigned dimensionality = address1.size();
		assert(address2.size() == dimensionality);
		unsigned distance = 0;
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			int diff = address1[i] - address2[i];
			distance += ((diff > 0) ? diff : -diff);
		}
		return distance;
	}
	int addressAdjacency(const Address& address1,const Address& address2)
	{
		// Addresses are adjacent if they differ at a single 
		// entry at most and the difference is either +/-1 or 
		// something else if one of the different address 
		// components is zero (to support torus network addresses).
		// This function returns:
		// 		0: if addresses are not adjacent
		//		k, k > 0: if addresses are adjacent and
		//				the different component is component k - 1, and
		//				address1[k - 1] < address2[k - 1]. or
		//				address1[k - 1] > 0 and address2[k - 1] == 0
		//		k, k < 0: if addresses are adjacent and
		//				the different component is component k - 1, and
		//				address1[k - 1] > address2[k - 1]. or
		//				address1[k - 1] == 0 and address2[k - 1] > 0
		unsigned dimensionality = address1.size();
		assert(address2.size() == dimensionality);
		bool diffFound = false;
		unsigned diffDimension = 0;
		for(unsigned i = 0 ; i < dimensionality ; ++i)
		{
			if(address1[i] != address2[i])
			{
				if(diffFound)
				{
					return 0;
				}
				diffFound = true;
				diffDimension = i;
			}
		}
		if(!diffFound)
		{
			return 0;
		}
		// one difference was found, check that it follows the 
		// adjacency rules
		int comp1 = address1[diffDimension];
		int comp2 = address2[diffDimension];
		if((comp1 == 0) && (comp2 > 0))
		{
			return -(diffDimension + 1);
		}
		if((comp2 == 0) && (comp1 > 0))
		{
			return (diffDimension + 1);
		}
		int diff = comp1 - comp2;
		if(diff == 1)
		{
			return -(diffDimension + 1);
		}
		if(diff == -1)
		{
			return (diffDimension + 1);
		}
		return 0;
	}
}

