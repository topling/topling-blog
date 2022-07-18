

> 我们使用 [YCSB](https://github.com/brianfrankcooper/YCSB) 对比测试了 MyTopling 和 Microsoft 的 Azure SQL，MyTopling 的性能大约是 Azure 的两倍。
>
> 我们采用和 Azure 官方几乎一样的测试方案，以保证测试结果真实、可信、可复现。
>
> Azure SQL 测试原文：[Testing performance of Azure SQL Database as a key-value store](https://devblogs.microsoft.com/azure-sql/azure-sql-database-as-a-key-value-store/)



## 一、测试结果



![ycsbtest](https://raw.githubusercontent.com/topling/topling-blog/main/resource/%E6%80%A7%E8%83%BD%E6%B5%8B%E8%AF%95/YCSB%20%E5%AF%B9%E6%AF%94%E6%B5%8B%E8%AF%95%20Azure%20%E5%92%8C%20MyTopling/ycsbtest.png)



## 二、测试环境



### 1. YCSB 版本

[0.16.0](https://github.com/brianfrankcooper/YCSB/releases/tag/0.16.0)

### 2. YCSB 客户端

**云厂商:** 阿里云

**CPU&内存:** 16核(vCPU)32 GiB

**操作系统:** Ubuntu 18.04 64位

**实例规格:** ecs.hfc7.4xlarge

### 3. JDBC 驱动器:   
**Azure：**[mssql-jdbc-8.4.1.jre11.jar](https://jar-download.com/artifacts/com.microsoft.sqlserver/mssql-jdbc/8.4.1.jre11/source-code)

**MyTopling：**[mysql-connector-java-8.0.26.jar](https://jar-download.com/artifacts/mysql/mysql-connector-java/8.0.26/source-code)

### 4. 云数据库：

**Azure:** Our first test was on a General Purpose database with **<font color=red>4 CPU cores and 20 GB of memory</font>**, using Gen5 hardware.

**MyTopling:** **<font color=red>4核(vCPU)16 GiB</font>** / Alibaba Cloud Linux 3.2104 LTS 64位 / ecs.hfg6.xlarge

### 5. `db.properties` 配置:   

**Azure:**

```pro
db.driver=com.microsoft.sqlserver.jdbc.SQLServerDriver
db.url=jdbc:sqlserver://server-name-here.database.windows.net:1433;databaseName=database-name-here;sendStringParametersAsUnicode=false;disableStatementPooling=false;statementPoolingCacheSize=10;responseBuffering=full
db.user=ycsb
db.passwd=strong-password-here
db.batchsize=5000
jdbc.batchupdateapi=true
```

**MyTopling:**

```properties
db.driver=com.mysql.cj.jdbc.Driver
db.url=jdbc:mysql://192.168.31.2:3306/test
db.user=ycsb
db.passwd=strong-password-here
db.batchsize=5000
jdbc.batchupdateapi=true 
```

#### 6. 表

```sql
CREATE TABLE usertable (
YCSB_KEY varchar(255) NOT NULL,
FIELD0 varchar(100) NOT NULL, 
FIELD1 varchar(100) NOT NULL, 
FIELD2 varchar(100) NOT NULL, 
FIELD3 varchar(100) NOT NULL, 
FIELD4 varchar(100) NOT NULL, 
FIELD5 varchar(100) NOT NULL, 
FIELD6 varchar(100) NOT NULL, 
FIELD7 varchar(100) NOT NULL, 
FIELD8 varchar(100) NOT NULL, 
FIELD9 varchar(100) NOT NULL,
CONSTRAINT pk_usertable PRIMARY KEY (YCSB_KEY)
);
```



#### 7. 工作负载(Workload)

使用 YCSB 默认之一 workloada，仅修改 recordcount 和 operationcount 值为 10000000。

```properties
recordcount=10000000
operationcount=10000000
workload=com.yahoo.ycsb.workloads.CoreWorkload

readallfields=true

readproportion=0.5
updateproportion=0.5
scanproportion=0
insertproportion=0

requestdistribution=zipfian
```



#### 8. YCSB 输出

**Azure:**

```
[OVERALL], RunTime(ms), 1089898
[OVERALL], *Throughput(ops/sec), 9175.170520544125
[READ], Operations, 4999607
[READ], *AverageLatency(us), 1106.8701749957547
[READ], MinLatency(us), 183
[READ], MaxLatency(us), 311807
[READ], 95thPercentileLatency(us), 2265
[READ], *99thPercentileLatency(us), 4767
[READ], Return=OK, 4999607
[UPDATE], Operations, 5000393
[UPDATE], *AverageLatency(us), 3176.099026216539
[UPDATE], MinLatency(us), 224
[UPDATE], MaxLatency(us), 313087
[UPDATE], 95thPercentileLatency(us), 7403
[UPDATE], *99thPercentileLatency(us), 15175
[UPDATE], Return=OK, 5000393 
```

**MyTopling:**

```
[OVERALL], RunTime(ms), 582640
[OVERALL], Throughput(ops/sec), 17163.256899629272
[READ], Operations, 5000993
[READ], AverageLatency(us), 1064.9375506024503
[READ], MinLatency(us), 174
[READ], MaxLatency(us), 1266687
[READ], 95thPercentileLatency(us), 3369
[READ], 99thPercentileLatency(us), 5151
[READ], Return=OK, 5000993
[UPDATE], Operations, 4999007
[UPDATE], AverageLatency(us), 1140.1147631919698
[UPDATE], MinLatency(us), 192
[UPDATE], MaxLatency(us), 1266687
[UPDATE], 95thPercentileLatency(us), 3509
[UPDATE], 99thPercentileLatency(us), 5375
[UPDATE], Return=OK, 4999007
```



## 三、为什么 MyTopling 这样快



### 1. 什么是 MyTopling

MyTopling 是拓扑岭([topling.cn](https://link.zhihu.com/?target=https%3A//topling.cn/))开发的 MySQL 兼容数据库。

MyTopling fork 自 facebook 的 [myrocks-8.0.23](https://link.zhihu.com/?target=https%3A//github.com/facebook/mysql-5.6/tree/fb-mysql-8.0.23)，进行了大幅修改：

1. 将存储引擎从 RocksDB 替换为 ToplingDB

2. 对 myrocks 的接入层进行了深度优化

3. 对 ToplingDB Transaction 进行了深度优化

4. 1. Transaction 代码虽然在 ToplingDB/RocksDB 中，但其主要是为 MyTopling/MyRocks 服务的

5. 支持 ToplingDB 的 [分布式 Compact](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/Distributed-Compaction)

6. 支持 ToplingDB 的 [CSPP_WBWI](https://link.zhihu.com/?target=https%3A//github.com/topling/cspp-wbwi)(CSPP WriteBatchWithIndex)

7. 支持 ToplingDB 的 [CSPP MemTable](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/ToplingCSPPMemTab)

8. 支持 ToplingDB 的 [ToplingFastTable](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/ToplingFastTable)

9. 支持 ToplingDB 的 [ToplingZipTable](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/ToplingZipTable)

10. 支持 ToplingDB 的 [引擎级监控（grafana）](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/grafana%E5%B1%95%E7%A4%BAtoplingdb%E8%BF%90%E8%A1%8C%E6%8C%87%E6%A0%87-%E6%89%8B%E5%8A%A8%E9%85%8D%E7%BD%AE)

11. 支持 ToplingDB 的 [引擎观测](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/WebView)

### 2. MyTopling 的 创建索引 及 bulk_load 加速

在 MyRocks 中，bulk_load 和 创建索引走的是同一套逻辑，其处理方式大致为：

1. 如果输入数据有序，直接使用 RocksDB 的 SstWriter 创建 SST 文件，然后将创建出来的 SST 文件注入到 LSM Tree 中
2. 如果输入数据无序，先对数据排序（内排+外排），然后再执行 1 中的步骤

在 sysbench 性能测试中，我们发现 prepare 阶段创建索引的速度很慢，进一步定位到是在内存中的排序很慢，其排序方式非常原始粗暴，使用了 `std::set`，并且在应该给 `std::set` 指定自定义 KeyComparator 的地方，把 RocksDB 中的 Comparator 指针放到每个 set 元素中。所以，即便使用最普通的优化方式，也能对性能有所提升，并降低内存用量。但我们最终进行了非常彻底的优化：

1. 在 ToplingDB SstWriter 中，允许乱序输入
2. 实现 AutoSortTable，以支持乱序输入
3. 修改 MyRocks 相关代码，支持乱序输入

这中间的核心是 AutoSortTable，我们知道，RocksDB 的 SST 是 **S**tatic **S**orted **T**able（也有称 Sorted String Table），创建 SST 时，输入数据必须是按 Key 有序的。支持乱序输入，其本质就是把排序从 SST 外部移到内部，貌似没啥可圈可点的。

然而，这里核心中的核心是：AutoSortTable 使用了 CSPP Trie，将 Key 逐个插入 CSPP Trie，比在内存中排序还要快，并且 CSPP Trie 只使用一整块内存，这意味着创建好的 CSPP Trie 数据结构可以通过一次 write 操作写到 SST 文件中。

最终效果是，优化后，索引创建的速度提升了 30 倍以上！

### 3. ToplingDB 的 Transaction 优化

1. 在 MyRocks 的 Transaction 中，原本其性能热点是 SkipList （SkipList MemTable 和 SkipList WBWI），我们将这两个都用 CSPP Trie 替换之后，这个性能热点就消失了
2. SkipList 性能热点消除后，原本不是性能热点的 Lock 管理，成了新的性能热点，这一块，主要通过 topling 的 hash_strmap 和 gold_hash_map 解决，并且，把调用链上的 std::string，改成了无需内存拷贝的 Slice

### 4. MyTopling 的 分布式 Compact

分布式 Compact 的核心是把 CompactionFilter 和 TablePropertiesCollector 需要的数据字典转移到 Compact Worker 结点上，这个，我们只需要仔细研读相关代码，将数据字典进行序列化/反序列化即可，并且，ToplingDB 提供了完善 SidePlugin 机制，使得实现这一点变得相对容易。

### 5. Compaction Worker 多进程化

MyRocks 中 CompactionFilter 和 TablePropertiesCollector 使用了一些全局变量，而我们的 Compact Worker 是多线程为多个 DB 实例服务的，全局变量在多线程中是共享的，这样就会导致来自多个 DB 的数据发生混淆。所以，我们将 Compact Worker 改成多线程的，将执行 Compact Job 的代码放到 fork 出来的新进程中，关于这一点，我们[在知乎上有个回答](https://www.zhihu.com/question/25390536/answer/2510470886)

### 6. 对上游 RocksDB 的 Pull Request

我们的诸多修改，有一些代码使用了 topling-core，对 RocksDB 的改动也较大，但还有一些修改没有外部依赖，我们将没有外部依赖的代码，都向上游官方 RocksDB 发起了 [Pull Request](https://link.zhihu.com/?target=https%3A//github.com/facebook/rocksdb/pulls/rockeet)
