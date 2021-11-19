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

int stoi_with_check(const std::string& db_index_s)
{
    // return -1 when db_index_s is invalid

    if(db_index_s.length() >= 10)
        return -1;
    for(auto ch : db_index_s)
        if(ch < '0' || ch > '9')
            return -1;
    return std::stoi(db_index_s);
}

void Worker(rocksdb::DB_MultiCF* &dbm)
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

            auto db_index = stoi_with_check(slices[1]);
            if(db_index < 0)
                std::cout << "invalid db_index" << std::endl;
            else if(db_index >= handles.size())
                std::cout << "db_index is out of range." << std::endl;
            else
            {
                now_handle = handles[db_index];
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
