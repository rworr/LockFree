//Interface for the producer-consumer tree

#ifndef _PCTREE_INTERFACE
#define _PCTREE_INTERFACE

#include "BasicBinaryTree.hpp"
#include <assert.h>

namespace Container
{
	class PCTree
	{
	public:
		//Constructor
		PCTree()
		{
		}

		//Constructor initializing the root of the tree
		PCTree(int val)
			: m_Tree(val)
		{
		}

		//virtual insert
		virtual void Insert(int val)
		{
			//cannot be called
			assert(false);
		}

		//virtual remove of the closest value
		virtual int Remove(int val)
		{
			//cannot be called
			assert(false);
			return 0;
		}

		void Check()
		{
			m_Tree.CheckTree();
		}

	protected:
		BasicBinaryTree m_Tree;
	};
}

#endif