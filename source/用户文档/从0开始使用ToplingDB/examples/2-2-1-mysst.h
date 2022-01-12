#define ROCKSDB_PLATFORM_POSIX
#include "table/block_based/block_based_table_factory.h"
namespace rocksdb
{

struct MyBlockBasedTableOptions : public BlockBasedTableOptions {};

class MyBlockBasedTableFactory : public BlockBasedTableFactory
{
public:
explicit MyBlockBasedTableFactory(
      const MyBlockBasedTableOptions& table_options = MyBlockBasedTableOptions());
      const char* Name() const;
~MyBlockBasedTableFactory() {};
};

}
