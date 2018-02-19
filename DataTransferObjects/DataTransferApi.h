#include <string>

namespace DataTransferApi
{
	class IMessage {
	public:
		IMessage() {}
		virtual ~IMessage() {}
		void setData(const std::string data)
		{
			_data = data;
		}
		std::string getData() const
		{
			return _data;
		}
	private:
		std::string _data;
	};

	//Declation
	template <class T, class TT = void>
	class Serialization;

	//Implementation, can be more elaborate
	template <class T>
	class Serialization<T, typename std::enable_if<std::is_base_of<IMessage, T>::value>::type> {
	public:
		static bool Serialize(const IMessage& msg, std::string& buffer) {
			// add code for other properties on IMessage
			buffer = msg.getData();
			return true;
		}
		static bool Deserialize(std::string& buffer, IMessage& msg) {
			// add code for other properties on IMessage
			msg.setData(buffer);
			return true;
		}
	};
}
#pragma once
