## 一、测试结果

### 1. Azure SQL

**load 阶段**

由于 Azure SQL 的 load 阶段结果未知，所以没有比较。

**run 阶段**

![azure-mytopling-run](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/YCSB%20对比测试%20MyTopling/azure-mytopling-run.png)



### 2. MyRocks

**load 阶段**

![myrocks-mytopling-load](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/YCSB%20对比测试%20MyTopling/myrocks-mytopling-load.png)

**run 阶段**

![myrocks-mytopling-run](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/YCSB%20对比测试%20MyTopling/myrocks-mytopling-run.png)



### 3. PolarDB

**load 阶段**

![polardb-mytopling-load](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/YCSB%20对比测试%20MyTopling/polardb-mytopling-load.png)

**run 阶段**

![polardb-mytopling-run](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/YCSB%20对比测试%20MyTopling/polardb-mytopling-run.png)



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

**MyTopling / MyRocks / PolarDB：**[mysql-connector-java-8.0.26.jar](https://jar-download.com/artifacts/mysql/mysql-connector-java/8.0.26/source-code)

### 4. 云数据库：

**Azure:** Our first test was on a General Purpose database with **<font color=red>4 CPU cores and 20 GB of memory</font>**, using Gen5 hardware.

**MyTopling / MyRocks / PolarDB:** **<font color=red>4核(vCPU)16 GiB</font>** / Alibaba Cloud Linux 3.2104 LTS 64位 / ecs.hfg6.xlarge

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

**MyTopling / MyRocks / PolarDB:**

```properties
db.driver=com.mysql.cj.jdbc.Driver
db.url=jdbc:mysql://192.168.31.2:3306/test
db.user=ycsb
db.passwd=strong-password-here
db.batchsize=5000
jdbc.batchupdateapi=true 
```

### 6. 表

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



### 7. 工作负载(Workload)

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



### 8. 测试输出

所有测试使用的线程数均为 20。

#### MyTopling

**load**

```
[OVERALL], RunTime(ms), 284570
[OVERALL], Throughput(ops/sec), 35140.7386583266
[TOTAL_GCS_PS_Scavenge], Count, 2959
[TOTAL_GC_TIME_PS_Scavenge], Time(ms), 2506
[TOTAL_GC_TIME_%_PS_Scavenge], Time(%), 0.8806269107776645
[TOTAL_GCS_PS_MarkSweep], Count, 0
[TOTAL_GC_TIME_PS_MarkSweep], Time(ms), 0
[TOTAL_GC_TIME_%_PS_MarkSweep], Time(%), 0.0
[TOTAL_GCs], Count, 2959
[TOTAL_GC_TIME], Time(ms), 2506
[TOTAL_GC_TIME_%], Time(%), 0.8806269107776645
[CLEANUP], Operations, 20
[CLEANUP], AverageLatency(us), 220.25
[CLEANUP], MinLatency(us), 138
[CLEANUP], MaxLatency(us), 998
[CLEANUP], 95thPercentileLatency(us), 216
[CLEANUP], 99thPercentileLatency(us), 998
[INSERT], Operations, 10000000
[INSERT], AverageLatency(us), 549.8476009
[INSERT], MinLatency(us), 209
[INSERT], MaxLatency(us), 36671
[INSERT], 95thPercentileLatency(us), 1083
[INSERT], 99thPercentileLatency(us), 1675
[INSERT], Return=OK, 10000000
```

**run**

```
[OVERALL], RunTime(ms), 426661
[OVERALL], Throughput(ops/sec), 23437.811283431107
[TOTAL_GCS_PS_Scavenge], Count, 2883
[TOTAL_GC_TIME_PS_Scavenge], Time(ms), 2586
[TOTAL_GC_TIME_%_PS_Scavenge], Time(%), 0.6061017997895285
[TOTAL_GCS_PS_MarkSweep], Count, 0
[TOTAL_GC_TIME_PS_MarkSweep], Time(ms), 0
[TOTAL_GC_TIME_%_PS_MarkSweep], Time(%), 0.0
[TOTAL_GCs], Count, 2883
[TOTAL_GC_TIME], Time(ms), 2586
[TOTAL_GC_TIME_%], Time(%), 0.6061017997895285
[READ], Operations, 4999532
[READ], AverageLatency(us), 824.545880694433
[READ], MinLatency(us), 178
[READ], MaxLatency(us), 810495
[READ], 95thPercentileLatency(us), 2145
[READ], 99thPercentileLatency(us), 3533
[READ], Return=OK, 4999532
[CLEANUP], Operations, 20
[CLEANUP], AverageLatency(us), 234.2
[CLEANUP], MinLatency(us), 165
[CLEANUP], MaxLatency(us), 967
[CLEANUP], 95thPercentileLatency(us), 226
[CLEANUP], 99thPercentileLatency(us), 967
[UPDATE], Operations, 5000468
[UPDATE], AverageLatency(us), 859.0911844651341
[UPDATE], MinLatency(us), 196
[UPDATE], MaxLatency(us), 771583
[UPDATE], 95thPercentileLatency(us), 2185
[UPDATE], 99thPercentileLatency(us), 3577
[UPDATE], Return=OK, 5000468
```



#### 	Azure SQL

**load**

```
未知
```

**run**

Azure 官方只给出了部分输出，如下

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



#### 	MyRocks

**load**

```
[OVERALL], RunTime(ms), 1019883
[OVERALL], Throughput(ops/sec), 9805.046265110803
[TOTAL_GCS_PS_Scavenge], Count, 5695
[TOTAL_GC_TIME_PS_Scavenge], Time(ms), 4911
[TOTAL_GC_TIME_%_PS_Scavenge], Time(%), 0.4815258220795915
[TOTAL_GCS_PS_MarkSweep], Count, 0
[TOTAL_GC_TIME_PS_MarkSweep], Time(ms), 0
[TOTAL_GC_TIME_%_PS_MarkSweep], Time(%), 0.0
[TOTAL_GCs], Count, 5695
[TOTAL_GC_TIME], Time(ms), 4911
[TOTAL_GC_TIME_%], Time(%), 0.4815258220795915
[CLEANUP], Operations, 20
[CLEANUP], AverageLatency(us), 212.05
[CLEANUP], MinLatency(us), 162
[CLEANUP], MaxLatency(us), 913
[CLEANUP], 95thPercentileLatency(us), 198
[CLEANUP], 99thPercentileLatency(us), 913
[INSERT], Operations, 10000000
[INSERT], AverageLatency(us), 1939.812077
[INSERT], MinLatency(us), 220
[INSERT], MaxLatency(us), 225151
[INSERT], 95thPercentileLatency(us), 5983
[INSERT], 99thPercentileLatency(us), 9503
[INSERT], Return=OK, 10000000
```



**run**

```
[OVERALL], RunTime(ms), 716470
[OVERALL], Throughput(ops/sec), 13957.318519965944
[TOTAL_GCS_PS_Scavenge], Count, 2884
[TOTAL_GC_TIME_PS_Scavenge], Time(ms), 2564
[TOTAL_GC_TIME_%_PS_Scavenge], Time(%), 0.3578656468519268
[TOTAL_GCS_PS_MarkSweep], Count, 0
[TOTAL_GC_TIME_PS_MarkSweep], Time(ms), 0
[TOTAL_GC_TIME_%_PS_MarkSweep], Time(%), 0.0
[TOTAL_GCs], Count, 2884
[TOTAL_GC_TIME], Time(ms), 2564
[TOTAL_GC_TIME_%], Time(%), 0.3578656468519268
[READ], Operations, 5000640
[READ], AverageLatency(us), 1315.4377993616818
[READ], MinLatency(us), 168
[READ], MaxLatency(us), 212735
[READ], 95thPercentileLatency(us), 4347
[READ], 99thPercentileLatency(us), 6995
[READ], Return=OK, 5000640
[CLEANUP], Operations, 20
[CLEANUP], AverageLatency(us), 248.4
[CLEANUP], MinLatency(us), 164
[CLEANUP], MaxLatency(us), 1024
[CLEANUP], 95thPercentileLatency(us), 405
[CLEANUP], 99thPercentileLatency(us), 1024
[UPDATE], Operations, 4999360
[UPDATE], AverageLatency(us), 1441.3606073577419
[UPDATE], MinLatency(us), 187
[UPDATE], MaxLatency(us), 209919
[UPDATE], 95thPercentileLatency(us), 4711
[UPDATE], 99thPercentileLatency(us), 7483
[UPDATE], Return=OK, 4999360
```



#### 	PolarDB

**load**

```
[OVERALL], RunTime(ms), 1043218
[OVERALL], Throughput(ops/sec), 9585.724172704075
[TOTAL_GCS_PS_Scavenge], Count, 4956
[TOTAL_GC_TIME_PS_Scavenge], Time(ms), 3357
[TOTAL_GC_TIME_%_PS_Scavenge], Time(%), 0.3217927604776758
[TOTAL_GCS_PS_MarkSweep], Count, 0
[TOTAL_GC_TIME_PS_MarkSweep], Time(ms), 0
[TOTAL_GC_TIME_%_PS_MarkSweep], Time(%), 0.0
[TOTAL_GCs], Count, 4956
[TOTAL_GC_TIME], Time(ms), 3357
[TOTAL_GC_TIME_%], Time(%), 0.3217927604776758
[CLEANUP], Operations, 20
[CLEANUP], AverageLatency(us), 337.75
[CLEANUP], MinLatency(us), 97
[CLEANUP], MaxLatency(us), 4787
[CLEANUP], 95thPercentileLatency(us), 117
[CLEANUP], 99thPercentileLatency(us), 4787
[INSERT], Operations, 10000000
[INSERT], AverageLatency(us), 2067.5827051
[INSERT], MinLatency(us), 404
[INSERT], MaxLatency(us), 206079
[INSERT], 95thPercentileLatency(us), 4767
[INSERT], 99thPercentileLatency(us), 14895
[INSERT], Return=OK, 10000000
```

**run**

```
[OVERALL], RunTime(ms), 468401
[OVERALL], Throughput(ops/sec), 21349.228545626505
[TOTAL_GCS_PS_Scavenge], Count, 1765
[TOTAL_GC_TIME_PS_Scavenge], Time(ms), 1255
[TOTAL_GC_TIME_%_PS_Scavenge], Time(%), 0.2679328182476126
[TOTAL_GCS_PS_MarkSweep], Count, 0
[TOTAL_GC_TIME_PS_MarkSweep], Time(ms), 0
[TOTAL_GC_TIME_%_PS_MarkSweep], Time(%), 0.0
[TOTAL_GCs], Count, 1765
[TOTAL_GC_TIME], Time(ms), 1255
[TOTAL_GC_TIME_%], Time(%), 0.2679328182476126
[READ], Operations, 4997696
[READ], AverageLatency(us), 612.3697041596768
[READ], MinLatency(us), 259
[READ], MaxLatency(us), 211711
[READ], 95thPercentileLatency(us), 1182
[READ], 99thPercentileLatency(us), 1816
[READ], Return=OK, 4997696
[CLEANUP], Operations, 20
[CLEANUP], AverageLatency(us), 366.15
[CLEANUP], MinLatency(us), 111
[CLEANUP], MaxLatency(us), 4791
[CLEANUP], 95thPercentileLatency(us), 180
[CLEANUP], 99thPercentileLatency(us), 4791
[UPDATE], Operations, 5002304
[UPDATE], AverageLatency(us), 1238.3440890437687
[UPDATE], MinLatency(us), 393
[UPDATE], MaxLatency(us), 207103
[UPDATE], 95thPercentileLatency(us), 2145
[UPDATE], 99thPercentileLatency(us), 3307
[UPDATE], Return=OK, 5002304
```

