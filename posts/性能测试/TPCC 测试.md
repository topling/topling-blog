

## 0. 前置

1. 在控制台创建 MyTopling 机器。
2. 在阿里云创建客户端。



客户端需要与 MyTopling 机器在同一个 VPC 下，后面使用内网连接。



## 1. 修改 MyTopling 配置（可选）

登录 MyTopling 机器，修改配置并重启 MyTopling。



## 2. 客户端配置

首先登录阿里云客户端。



### i) 安装必要程序

```shell
sudo yum install -y java mysql
```



### ii) 下载并进入 tpcc 测试程序目录

```shell
mkdir tpcc
wget https://docs-aliyun.cn-hangzhou.oss.aliyun-inc.com/assets/attach/183466/cn_zh/1620452585651/tpcc.tar.gz
tar -zxvf tpcc.tar.gz -C tpcc
cd tpcc
```



### iii) 修改配置文件

```shell
vim run/props.mysql
```

如下：

```properties
// --------- env config --------- // 
db=mysql
driver=com.mysql.jdbc.Driver
conn=jdbc:mysql://10.83.4.206:3306/tpcc?useSSL=false&useServerPrepStmts=false&useConfigs=maxPerformance&rewriteBatchedStatements=true
user=admin
password=test123

// warehouse 数量
warehouses=1000
// 导入数据的并发数，每100并发预计产生2万TPS，可以结合目标TPS能力调整并发
// runLoader.sh的jvm内存，100并发默认为4 GB，500并发建议设置为16 GB
loadWorkers=100

// TPC-C压测并发数
terminals=1000
// 压测时间，单位分钟
runMins=10
// ---------- default config ------- //
//To run specified transactions per terminal- runMins must equal zero
runTxnsPerTerminal=0
//Number of total transactions per minute
limitTxnsPerMin=0

//Set to true to run in 4.x compatible mode. Set to false to use the
//entire configured database evenly.
terminalWarehouseFixed=true

//The following five values must add up to 100
//The default percentages of 45, 43, 4, 4 & 4 match the TPC-C spec
newOrderWeight=45
paymentWeight=43
orderStatusWeight=4
deliveryWeight=4
stockLevelWeight=4

// Directory name to create for collecting detailed result data.
// Comment this out to suppress.
resultDirectory=my_result_%tY-%tm-%td_%tH%tM%tS

// osCollectorScript=./misc/os_collector_linux.py
// osCollectorInterval=1
// osCollectorSSHAddr=user@dbhost
// osCollectorDevices=net_eth0 blk_sda
```

改小数据量的话我这里主要改的是下面四个字段：

1. `warehouses`
2. `loadWorkers`
3. `terminals`
4. `runMins`



### iv) 修改建表文件

```
vim run/sql.common/tableCreates.sql
```

在 `:` 执行并保存：

```
%s/dbpartition.*/;/g
```



### v) 修改 MyTopling 密码和权限内容

在客户端登录 MyTopling

```shell
mysql -h10.83.4.206 -uadmin -p123456
```

修改密码：

```sql
ALTER USER 'admin'@'%' identified by 'test123';
ALTER USER 'admin'@'%' IDENTIFIED WITH mysql_native_password BY 'test123';
flush privileges;
```

创建名为 tpcc 的数据库，然后退出：

```sql
CREATE DATABASE tpcc;
exit
```



## 3. 开始测试

进入阿里云客户端中的 tpcc 目录

```shell
cd run
```

**第一阶段**

```shell
./runDatabaseBuild.sh props.mysql
```

**第二阶段**

```shell
./runBenchmark.sh props.mysql
```



