## 从0开始使用ToplingDB

### 部署安装ToplingDB

0. 服务器环境

    操作系统： CentOS Linux release 8.4.2105

    g++版本： g++ (GCC) 8.4.1 20200928 (Red Hat 8.4.1-1)

    

1. 安装相关依赖

   ToplingDB基于[RocksDB](https://github.com/facebook/rocksdb)构建，我们需要用到压缩库[snappy](http://google.github.io/snappy/)和命令行参数解析工具[gflags](https://github.com/gflags/gflags)。除此之外，在编译的过程中，还需要用到libaio的开发包。

   - 安装snappy：

     ```bash
     sudo yum install snappy snappy-devel
     ```

   - 安装gflags：

     - 对于CentOS 8：

       ```bash
       sudo dnf --enablerepo=powertools install gflags-devel
       ```

     - 对于CentOS 7（需要 [EPEL](https://fedoraproject.org/wiki/EPEL)）：

       ```bash
       sudo yum install gflags-devel
       ```
   
   - 安装libaio-devel：
   
     ```bash
     sudo yum install libaio-devel
     ```

   

2. 安装ToplingDB

    - 获取项目源代码：

      ```bash
      cd ~
      git clone https://github.com/topling/toplingdb.git
      ```

    - 更新依赖的子项目：

      ```bash
      cd toplingdb
      git submodule update --init --recursive
      ```

    - 编译安装动态库：

      ```bash
      make shared_lib
      sudo make install
      ```

    - 设置环境变量：

      除了librocksdb.so之外，我们还会用到[topling-zip](https://github.com/topling/topling-zip)编译生成的libterark-zbs-r.so等动态库。在刚才的make过程中，topling-zip已被克隆到`toplingdb/sideplugin`目录下，它编译得到的动态库位于`topling-zip/build/Linux-x86_64-g++-8.4-bmi2-1/lib_shared`。

      打开文件`~/.bashrc`，在文件的末尾增加下列两行：
      
      ```bash
      export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
      export LD_LIBRARY_PATH=~/toplingdb/sideplugin/topling-zip/build/Linux-x86_64-g++-8.4-bmi2-1/lib_shared:$LD_LIBRARY_PATH
      ```
      
      保存后，执行以下命令，更新我们的设置：
      
      ```bash
      source ~/.bashrc
      ```
      
      需要注意的是，`Linux-x86_64-g++-8.4-bmi2-1`这一目录名称是根据编译环境而自动命名的。若您的编译环境与本文环境不同，您需要自行查看具体的目录，并调整之前设置的环境变量路径。



### 通过配置文件打开数据库

ToplingDB是一个嵌入式数据库，数据库的库文件直接链接在应用程序中，应用程序通过调用API来进行数据库的读写操作。

在本文的所有示例中，我们将数据库放置在路径`/home/topling/db/`下，也就是用户主目录下的`db`文件夹中。所有编写的代码和配置文件也放在用户主目录`/home/topling/`下。

1. 创建配置文件和数据库目录

   执行下列命令建立存放数据库的文件夹。

   ```shell
   cd ~
   mkdir -p db
   mkdir -p db/db_mcf
   ```

   在同一目录下创建配置文件`toplingconf.json`，然后找到我们的[示例配置文件](https://github.com/topling/topling-blog/blob/main/examples/%E4%BB%8E0%E5%BC%80%E5%A7%8B%E4%BD%BF%E7%94%A8ToplingDB/1-1-toplingconf.json)，将它里面的配置信息复制进来。

   接下来，修改配置信息中的数据库路径信息`path`，它位于最末尾的`db_mcf`字段中。将它修改为你自己的用户主目录下的`db`文件夹下的`db_mcf`。

   ```
   "path": "/home/topling/db/db_mcf"
   ```

   更多关于配置文件的信息，请参阅[配置系统介绍](https://github.com/topling/rockside/wiki/Configuration-File)。

   
   
2. 创建操作数据库的.cc/.cpp/.cxx文件

   在用户主空间下，创建包含main函数的文件`sample.cpp`，加载我们会用到的头文件`topling/side_plugin_repo.h`，以及标准输入输出流的头文件`iostream`。

   ```C++
   #include "topling/side_plugin_factory.h"
   #include <iostream>
   ```

   在主函数中，创建一个`rocksdb::SidePluginRepo`类的实例`repo`。调用它的成员函数`ImportAutoFile`，从我们刚才写好的配置文件中加载配置信息。

   ```C++
   rocksdb::SidePluginRepo repo;    // Repo represents of ConfigRepository
   repo.ImportAutoFile("/home/topling/toplingconf.json");
   ```

   在示例的配置信息中，打开的数据库是`db_mcf`，这是一个包含多个ColumnFamily的DB，对应类型`rocksdb::DB_MultiCF`。创建一个该类型的指针`dbm`来接收打开的数据库，并将返回的`rocksdb::Status`中的信息打印出来。如果返回的是OK，则表示打开成功。

   ```C++
   #include "topling/side_plugin_factory.h"
   #include <iostream>
   
   int main()
   {
       rocksdb::SidePluginRepo repo;
       repo.ImportAutoFile("/home/topling/toplingconf.json");
   
       rocksdb::DB_MultiCF *dbm;
       auto status = repo.OpenDB(&dbm);
       std::cout << status.ToString() << std::endl;
   
       return 0;
   }
   ```

   

3. 编译

   使用以下指令进行编译，输出可执行文件`sample.out`。

   ```
   g++ sample.cpp -I ~/toplingdb/sideplugin/rockside/src -I ~/toplingdb -I ~/toplingdb/sideplugin/topling-zip/src -I ~/toplingdb/sideplugin/topling-zip/boost-include -l:librocksdb.so -DSIDE_PLUGIN_WITH_YAML=1  -DROCKSDB_NO_DYNAMIC_EXTENSION=1 -o sample.out
   ```

   使用命令`./sample.out`执行生成的二进制文件。不出意外，我们将看到终端打印出OK，这表示我们正确地打开了数据库。

   

5. 对数据库的简单读写操作

   在打开数据库后，`dbm`中有两个重要的成员变量：指向数据库实例的指针`db`和储存所有ColumnFamilyHandle的vector容器`cf_handles`。

   ```C++
   auto db = dbm -> db;
   auto handles = dbm -> cf_handles;
   ```

   通过它们就可以像操作RocksDB一般，对ToplingDB进行读写了。如果我们在此基础上增加对输入命令的解析，就成了一个简单的服务式的[KV数据库程序](https://github.com/topling/topling-blog/blob/main/examples/%E4%BB%8E0%E5%BC%80%E5%A7%8B%E4%BD%BF%E7%94%A8ToplingDB/1-4-sample.cpp)。

   ```C++
   // write
   db -> Put(rocksdb::WriteOptions(), handles[0], rocksdb::Slice("test"), rocksdb::Slice("default_cf");
   db -> Put(rocksdb::WriteOptions(), handles[1], rocksdb::Slice("test"), rocksdb::Slice("custom_cf");
   
   //read
   std::string value1 , value2;
   db -> Get(rocksdb::ReadOptions(), handles[0], rocksdb::Slice("test"), &value1);
   db -> Get(rocksdb::ReadOptions(), handles[1], rocksdb::Slice("test"), &value2);
   std::cout << value1 << std::endl;
   std::cout << value2 << std::endl;
   
   //delete
   status = db -> Delete(rocksdb::WriteOptions(), handles[0], rocksdb::Slice("test"));
   std::cout << status.ToString() << std::endl;
   status = db -> Delete(rocksdb::WriteOptions(), handles[0], rocksdb::Slice("not exist"));
   std::cout << status.ToString() << std::endl;
   ```




### 更换SST Table

ToplingDB支持[旁路插件化](https://github.com/topling/rockside/wiki)，只通过更改配置文件就可以更换SST文件的TableFactory，无需修改代码。

- 使用RocksDB内置的SST

  修改配置文件中TableFactory的部分，增加不同Table类型的配置。

  ```json
  "TableFactory": {
      "block_based": {
          "class": "BlockBasedTable",
          "params": {
      
          }
      },
      "cuckoo": {
          "class": "CuckooTable",
          "params": {
              
          }
      },
      "plain": {
          "class": "PlainTable",
          "params": {
              
          }
      }
  },
  ```

  然后在database的部分中，使用我们新设置的table：

  ```json
  "SliceTransform": {
      "default": {
          "class" : "FixedPrefixTransform",
          "params" :{
              "prefix_len" : 10
          }
      }
  },
  "database": {
      ......
      
      "column_families": {
          "default": "$default",
          "custom_cf" : {
              "max_write_buffer_number": 4,
              "target_file_size_base": "16M",
              "target_file_size_multiplier": 2,
              "table_factory": "block_based",
              "ttl": 0
          },
          "cuckoo_cf" : {
              "table_factory": "cuckoo"
          },
          "plain_cf" : {
              "table_factory": "plain",
              "prefix_extractor" : "$default"
          }
      },
  }
  ```

  直接运行我们之前的程序，现在打开的数据库中，`cuckoo_cf`和`plain_cf`这两个ColumnFamily就已经使用了新的Table而不是默认的BlockBasedTable。

  如果您在这一步遇到了问题，也可以参考[2-1-toplingconf.json](https://github.com/topling/topling-blog/blob/main/examples/%E4%BB%8E0%E5%BC%80%E5%A7%8B%E4%BD%BF%E7%94%A8ToplingDB/2-1-toplingconf.json)。

  

- 使用第三方SST文件

  只需要通过`ROCKSDB_FACTORY_REG`注册第三方的Factory，就可以像使用RocksDB内置类型一样使用第三方SST文件。

  为了进行一个简单的示范，我们稍微包装一下BlockBasedTable，拿它当作一个第三方SST文件。

  

  1. 创建[mysst.h](https://github.com/topling/topling-blog/blob/main/examples/%E4%BB%8E0%E5%BC%80%E5%A7%8B%E4%BD%BF%E7%94%A8ToplingDB/2-2-1-mysst.h)

     ```C++
     // mysst.h
     
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
     ```

     

  2. 创建[mysst.cpp](https://github.com/topling/topling-blog/blob/main/examples/%E4%BB%8E0%E5%BC%80%E5%A7%8B%E4%BD%BF%E7%94%A8ToplingDB/2-2-2-mysst.cpp)

     ```C++
     // mysst.cpp
     
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
     ```

     可以看到MyBlockBasedTable只是继承了BlockBasedTable而已，没有其它的改动。只不过当我们使用MyBlockBasedTable时，执行它的构造函数会打印出"Using MyBlockBasedTableFactory"。

     

  3. 注册MyBlockBasedTable

     在`mysst.cpp`文件中，增加以下部分：

     ```C++
     #include "topling/side_plugin_factory.h"
     namespace rocksdb
     {
     
     std::shared_ptr<TableFactory> ThirdSSTExample(const json& js , const SidePluginRepo& repo)
     {
         return std::make_shared<MyBlockBasedTableFactory>(MyBlockBasedTableOptions());
     }
     ROCKSDB_FACTORY_REG("MyBlockBased", ThirdSSTExample);
     }
     ```
  
     修改完成后的代码可以参考[mysst.cpp](https://github.com/topling/topling-blog/blob/main/examples/%E4%BB%8E0%E5%BC%80%E5%A7%8B%E4%BD%BF%E7%94%A8ToplingDB/2-2-3-mysst.cpp)。
  
     
  
     这里为了方便起见，我们总是使用默认的配置项来构造MyBlockBasedTable。在实际使用中，您应该通过js中保存的json信息来构造您使用的TableFactory，它类似这样：
  
     ```C++
     std::shared_ptr<TableFactory> ThirdSSTExample(const json& js , const SidePluginRepo& repo)
     {
     	ThirdTableOptions table_options;
     	
         // some code for modifying table_options by json
         ...
         ...
         
         return std::make_shared<ThirdTableFactory>(table_options);
     }
     ROCKSDB_FACTORY_REG("MyBlockBased", ThirdSSTExample);
     ```
  
     
  
  7. 编译生成libmysst.so

     执行以下指令进行编译，生成自定义插件MyBlockBasedTable的动态库libmysst.so：

     ```bash
     g++ mysst.cpp -I ~/toplingdb -I ~/toplingdb/sideplugin/rockside/src -I ~/toplingdb/sideplugin/topling-zip/src -I ~/toplingdb/sideplugin/topling-zip/boost-include -l:librocksdb.so -fPIC -shared -o libmysst.so
     ```

     
  
  5. 动态加载libmysst.so：
  
     设置环境变量`LD_PRELOAD`后，直接运行我们之前的可执行程序`sample.out` :

     ```bash
     LD_PRELOAD=./libmysst.so ./sample.out
     ```
  
     此时MyBlockBasedTable已经注册进ToplingDB，现在就可以像之前使用RocksDB内置的PlainTable、CuckooTable一般，直接在配置项中启用MyBlockBasedTable了。
  
     在配置文件中进行如下修改，将内置类型`BlockBasedTable`改为第 3 步中，我们用`ROCKSDB_FACTORY_REG`宏注册的名称 MyBlockBased。
  
     ```json
     "TableFactory": {
         "block_based": {
             "class": "MyBlockBased",
             "params": {
                 
             }
         },
         
         ...
     }
     ```
  
     再次运行`sample.out`（不要忘记设置`LD_PRELOAD`！），就能看到MyBlockBasedTable在构造函数中打印的提示信息了。
  



### 基于AnyPlugin进行HTML展示（待补充进一步的示例）

为了方便，本示例直接在`sample.cpp`上进行修改，没有单独将HTML展示插件编译为单独的动态库。更多有关AnyPlugin的信息，请参阅[AnyPlugin](https://github.com/topling/rockside/wiki/AnyPlugin)。

1. 定义AnyPlugin派生类

   在`rocksdb`命名空间内，定义`AnyPlugin`的派生类`HtmlShowExample`，并修改它的`ToString`函数。`ToString` 函数的返回值是 std::string 类型，其返回的 string 字符串，会被**无差别地**打印在浏览器中。

   ```C++
   namespace rocksdb {
   class HtmlShowExample : public AnyPlugin 
   {
   public:
       void Update(const json&, const SidePluginRepo&) {}
       std::string ToString(const json& dump_options, const SidePluginRepo&) const 
       {
           return "This is an example of HTML show.";
       }
       const char* Name() const
       {
           return "HtmlShowExample";
       }
   };
   }
   ```

   定义派生类`HtmlShowExample`之后，仍然在`rocksdb`命名空间中，使用下列的宏将其注册。

   ```C++
   ROCKSDB_REG_DEFAULT_CONS(HtmlShowExample, AnyPlugin);
   ROCKSDB_REG_AnyPluginManip("HtmlShowExample");
   ```

2. 在主函数中增加http相关服务的调用

   在加载配置文件后，调用repo的成员函数`StartHttpServer`开启http服务。与打开DB相似，我们也可以打印出返回的`rocksdb::Status`的相关信息作为参考。

   ```
   auto http_status = repo.StartHttpServer();
   std::cout << http_status.ToString() << std::endl;
   ```

   为了能够在运行时间内看到运行效果，我们需要延长程序的运行时长，因此这里直接在最后加了一个死循环。修改后的`topling.cpp`如下。

   ```
   #include "topling/side_plugin_factory.h"
   #include <iostream>
   
   namespace rocksdb {
   class HtmlShowExample : public AnyPlugin 
   {
   public:
       void Update(const json&, const SidePluginRepo&) {}
       std::string ToString(const json& dump_options, const SidePluginRepo&) const 
       {
           return "This is an example of HTML show.";
       }
       const char* Name() const
       {
           return "A Test";
       }
   };
   
   ROCKSDB_REG_DEFAULT_CONS(HtmlShowExample, AnyPlugin);
   ROCKSDB_REG_AnyPluginManip("HtmlShowExample");
   }
   
   int main()
   {
       rocksdb::SidePluginRepo repo;
       repo.ImportAutoFile("/home/caoximeng/toplingconf.json");
   
       rocksdb::DB_MultiCF *dbm;
       auto status = repo.OpenDB(&dbm);
       std::cout << status.ToString() << std::endl;
   
       auto http_status = repo.StartHttpServer();
       std::cout << http_status.ToString() << std::endl;
       for( ; ; );
   
       return 0;
   }
   ```

3. 修改配置文件

   有两处地方需要修改。

   第一处是最开头的http字段，修改path为`/home/caoximeng/db`。

   第二处是在json文件的最外层中，增加我们的展示插件信息。

   ```
   {
   	"AnyPlugin": {
       "html-show-example": "HtmlShowExample"
   	},
       "http": {
         "document_root": "/home/caoximeng/db",
         "listening_ports": "8081"
       },
       .......
   }
   ```

4. 编译并运行项目

   使用我们之前的编译指令编译修改后的`topling.cpp`，并执行程序。

   我们在配置文件中设置的监听端口为8081，因此在浏览器中输入`127.0.0.1:8081/AnyPlugin/html-show-example`，即可看到展示信息。

   如果你不是在本地上执行程序，将`127.0.0.1`更改为你的机器的访问ip。如果执行程序打印的信息均为OK，但你无法打开页面，请检查防火墙设置。

