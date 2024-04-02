
#include "network.h"
#include "stdio.h"

int main(int argc,char** argv)
{
	printf("nocsim simulator\n");
	std::vector<unsigned> sizes(3,0);
	sizes[0] = 10;
	sizes[1] = 15;
	sizes[2] = 12;
	network::TorusNetwork network(sizes);
	network.build(4,4);
	return 0;
}
