#include <stdlib.h>
#include <../include/mpool.hpp>
#include <../include/varsize_mpool.hpp>

struct sample_data
{
	int value;
};

using namespace std;
using namespace evl::utility;

int main(int argc, char* argv[])
{
	mpool::MPoolInfo* pool = mpool::mpool_get_global();
	pool->chunk_size = 2048;
	pool->max_cell_size = 200;
	mpool::mpool_init(pool);
	
	void* block = mpool::mpool_alloc(pool, 32);
	if (block != 0)
	{
		mpool::mpool_free(pool, block, 32);
	}

	//////////////////////////////////////////////////////////////////////////
	// test varsize_mpool
	VarsizeMPool vmpool;
	if (vmpool.Init(256))
	{
		void* b = vmpool.Alloc(34);
		if (b != 0)
		{
			vmpool.Free(b, 34);
		}
	}

	system("PAUSE");

	return 0;
}