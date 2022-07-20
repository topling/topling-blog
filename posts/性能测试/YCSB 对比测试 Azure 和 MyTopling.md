

> 我们使用 [YCSB](https://github.com/brianfrankcooper/YCSB) 对比测试了 MyTopling 和 Microsoft 的 Azure SQL，MyTopling 的性能大约是 Azure 的两倍。
>
> 我们采用和 Azure 官方几乎一样的测试方案，以保证测试结果真实、可信、可复现。
>
> Azure SQL 测试原文：[Testing performance of Azure SQL Database as a key-value store](https://devblogs.microsoft.com/azure-sql/azure-sql-database-as-a-key-value-store/)



## 一、测试结果



### 1. MyTopling VS. Azure SQL

Azure 的 load 阶段数据未知，所以没有比较。

run 阶段：

![ycsbtest](https://raw.githubusercontent.com/topling/topling-blog/main/resource/%E6%80%A7%E8%83%BD%E6%B5%8B%E8%AF%95/YCSB%20%E5%AF%B9%E6%AF%94%E6%B5%8B%E8%AF%95%20Azure%20%E5%92%8C%20MyTopling/ycsbtest.png)



### 2. MyTopling VS. MyRocks









## 二、为什么 MyTopling 这样快



MyTopling fork 自 facebook 的 [myrocks-8.0.23](https://link.zhihu.com/?target=https%3A//github.com/facebook/mysql-5.6/tree/fb-mysql-8.0.23)，进行了大幅修改：

1. 将存储引擎从 RocksDB 替换为 ToplingDB
2. 对 myrocks 的接入层进行了深度优化
3. 对 ToplingDB Transaction 进行了深度优化
4. Transaction 代码虽然在 ToplingDB/RocksDB 中，但其主要是为 MyTopling/MyRocks 服务的

5. 支持 ToplingDB 的 [分布式 Compact](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/Distributed-Compaction)
6. 支持 ToplingDB 的 [CSPP_WBWI](https://link.zhihu.com/?target=https%3A//github.com/topling/cspp-wbwi)(CSPP WriteBatchWithIndex)
7. 支持 ToplingDB 的 [CSPP MemTable](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/ToplingCSPPMemTab)
8. 支持 ToplingDB 的 [ToplingFastTable](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/ToplingFastTable)
9. 支持 ToplingDB 的 [ToplingZipTable](https://link.zhihu.com/?target=https%3A//github.com/topling/rockside/wiki/ToplingZipTable)
10. MyTopling 优化后的索引创建的速度提升了 30 倍以上
11. MyTopling 在 Transaction 中使用 CSPP Tire 替换了 SkipList





## 三、测试环境



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



