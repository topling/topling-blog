## 1. YCSB 客户端

版本：0.16.0

客户端：16C32G

## 2. Azure SQL 配置

Our first test was on a General Purpose database with 4 CPU cores and 20 GB of memory, using Gen5 hardware. General Purpose databases use Azure Premium storage, which is remote (network attached) with respect to the machine running the database engine.

## 3. MyTopling 配置

4C16G



## 4. DB 配置

```properties
db.driver=com.mysql.cj.jdbc.Driver
db.url=jdbc:mysql://192.168.31.2:3306/test
db.user=dell3test
db.passwd=dell3test
db.batchsize=5000
jdbc.batchupdateapi=true 
```

## 5. 表

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



## 6. 工作负载

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



## 7. 结果



Azure SQL

```
[OVERALL], RunTime(ms), 1089898
[OVERALL], Throughput(ops/sec), 9175.170520544125
[READ], Operations, 4999607
[READ], AverageLatency(us), 1106.8701749957547
[READ], MinLatency(us), 183
[READ], MaxLatency(us), 311807
[READ], 95thPercentileLatency(us), 2265
[READ], 99thPercentileLatency(us), 4767
[READ], Return=OK, 4999607
[UPDATE], Operations, 5000393
[UPDATE], AverageLatency(us), 3176.099026216539
[UPDATE], MinLatency(us), 224
[UPDATE], MaxLatency(us), 313087
[UPDATE], 95thPercentileLatency(us), 7403
[UPDATE], 99thPercentileLatency(us), 15175
[UPDATE], Return=OK, 5000393 
```



MyTopling

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



