
#include <map>

namespace LockFree
{
	template<typename KeyType, typename ValueType>
	class Map
	{
	public:
		//Constructor
		Map()
			: m_Map((new std::map<KeyType, ValueType>()))
		{
		}

		//Destructor
		~Map()
		{
			delete m_Map;
		}

		//Lock-free insertion by copying
		void Insert(KeyType key, ValueType value)
		{

		}

	private:
		std::map<KeyType, ValueType>* m_Map;
	};
}