#include "mysst.h"
#include <iostream>
#include "topling/side_plugin_factory.h"

namespace rocksdb
{

MyBlockBasedTableFactory::MyBlockBasedTableFactory(const MyBlockBasedTableOptions& _table_options)
     : BlockBasedTableFactory(_table_options) 
     {
         std::cout << "Using MyBlockBasedTableFactory" << std::endl;
     }

const char* MyBlockBasedTableFactory::Name() const
{
    return "MyBlockBasedTableFactory";
};

std::shared_ptr<TableFactory> ThirdSSTExample(const json& js , const SidePluginRepo& repo)
{
    return std::make_shared<MyBlockBasedTableFactory>(MyBlockBasedTableOptions());
}
ROCKSDB_FACTORY_REG("MyBlockBased", ThirdSSTExample);
}