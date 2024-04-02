// NoCSim
// Ahmed Hussein (amhussein4@gmail.com)
// April 2nd 2024

#ifndef PACKET_H_
#define PACKET_H_

namespace network
{
	class Router;
	class Packet
	{
	public:
		Packet(unsigned size,Router* destination);
		Packet(const Packet& packet) = delete;
		Packet(Packet&& packet) = delete;
		~Packet();
		Packet& operator=(const Packet& packet) = delete;
		Packet& operator=(Packet&& packet) = delete;
		const Router* destination() const;

	private:
		unsigned size_{0};
		Router* destination_{nullptr};
	};
}

#endif
