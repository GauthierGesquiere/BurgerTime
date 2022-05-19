#pragma once
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

		//Use this if you want to get the dataType, not the Data* member variable.
		template<typename T>
		const T& GetData() const
		{
			return static_cast<EventData<TAPE_ERASE>*>(pDataType)->DataType;
		}

		std::string Message{};
		Data* pDataType{ nullptr };

	};
}

