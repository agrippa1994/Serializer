#include "Serializer.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/access.hpp>

#define SERIALIZER(x) friend class boost::serialization::access; template<typename T> void serialize(T& x, const int)

namespace Pakets
{
	struct Message
	{
	public:
		enum : int {
			Information
		} msg;
	private:
		SERIALIZER(ar) {
			ar & msg;
		}
	};

	struct Information
	{
		std::string version;
		
	private:
		SERIALIZER(ar) { ar & version; }
	};
}


int main()
{
	std::string g_data;

	{
		Serializer stream;
		stream << Pakets::Message({ Pakets::Message::Information }) << Pakets::Information({ "1.0" });

		std::cout << stream.GetData() << std::endl;

		g_data = stream.GetData();
	}
	{
		Serializer stream(g_data.c_str(), g_data.length(), false);
		Pakets::Message msg;
		
		stream >> msg;
		if (msg.msg == Pakets::Message::Information)
		{
			Pakets::Information info;
			stream >> info;

			std::cout << "Die Info ist: " << info.version << std::endl;
		}
	}

	std::cin.get();
}

