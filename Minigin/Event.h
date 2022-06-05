#pragma once
#include <string>

namespace dae
{
	//Workaround for template
	struct Data
	{
	};

	template<typename T>
	struct EventData : Data
	{
		EventData(T data)
			: DataType{ data }
		{}

		T DataType{};
	};

	struct Event
	{
		template<typename T>
		Event(std::string message, T* dataType)
		: Message{ message }
		, pDataType{ new EventData<T>(dataType) }
		{}

		explicit Event(std::string message)
			: Message{ message }
		{}
		~Event();
		Event(Event& other) = delete;
		Event(Event&& other) = delete;
		Event operator=(Event& other) = delete;
		Event operator=(Event&& other) = delete;

		//Use this if you want to get the dataType, not the Data* member variable.
		template<typename T>
		const T& GetData() const
		{
			return static_cast<EventData<T>*>(pDataType)->DataType;
		}

		std::string Message{};
		Data* pDataType{ nullptr };

	};
}

