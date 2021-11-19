#include "mysst.h"
#include <iostream>

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

}