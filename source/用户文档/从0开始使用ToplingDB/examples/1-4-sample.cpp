#include "topling/side_plugin_factory.h"
#include "rocksdb/db.h"
#include <iostream>

std::vector<std::string> split(const char separate , const std::string& line)
{
    std::vector<std::string> ret;

    int lastPos = 0;
    while(lastPos < line.length())
    {
        auto newPos = line.find(separate , lastPos);

        if(newPos == std::string::npos)
            newPos = line.length();
        if(newPos - lastPos != 0)
            ret.push_back(std::move(line.substr(lastPos , newPos - lastPos)));
        lastPos = newPos + 1;
    }

    return ret;
}

void Worker(rocksdb::DB_MultiCF* dbm)
{
    auto db = dbm -> db;
    auto handles = dbm -> cf_handles;
    auto now_handle = handles[0]; 
    std::string line , value;

    for( ; ; )
    {
        std::getline(std::cin , line);
        auto slices = split(' ' , line);
        if(slices.empty())
            continue;
        
        if(slices[0] == "select")
        {
            if(slices.size() != 2)
            {
                std::cout << "syntax error!" << std::endl;
                continue;
            }

            auto selected_cf_handle = (*dbm)[slices[1]];
            if(!selected_cf_handle)
                std::cout << "invalid db_name" << std::endl;
            else
            {
                now_handle = selected_cf_handle;
                std::cout << "OK" << std::endl;
            }
        }
        else if(slices[0] == "set")
        {
            if(slices.size() != 3)
            {
                std::cout << "syntax error!" << std::endl;
                continue;
            }
            auto status = db -> Put(rocksdb::WriteOptions() , now_handle , rocksdb::Slice(slices[1]) , rocksdb::Slice(slices[2]));
            std::cout << status.ToString() << std::endl;
        }
        else if(slices[0] == "get")
        {
            if(slices.size() != 2)
            {
                std::cout << "syntax error!" << std::endl;
                continue;
            }
            auto status = db -> Get(rocksdb::ReadOptions() , now_handle , rocksdb::Slice(slices[1]) , &value);
            if(!status.ok())
                std::cout << status.ToString() << std::endl;
            else std::cout << value << std::endl;
        }
        else if(slices[0] == "delete")
        {
            if(slices.size() != 2)
            {
                std::cout << "syntax error!" << std::endl;
                continue;
            }
            auto status = db -> Delete(rocksdb::WriteOptions() , now_handle , rocksdb::Slice(slices[1]));
            std::cout << status.ToString() << std::endl;
        }
        else if(slices[0] == "quit")
        {
            if(slices.size() != 1)
            {
                std::cout << "syntax error!" << std::endl;
                continue;
            }
            db -> Close();
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        else
        {
            std::cout << "syntax error!" << std::endl;
        }
    }
}

int main()
{
    rocksdb::SidePluginRepo repo;
    repo.ImportAutoFile("/home/topling/toplingconf.json");

    rocksdb::DB_MultiCF *dbm;
    auto status = repo.OpenDB(&dbm);
    if(status.ok())
    {
        Worker(dbm);
        delete dbm;
    }
    else std::cout << status.ToString() << std::endl;

    return 0;
}
