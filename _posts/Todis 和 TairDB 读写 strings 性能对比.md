# Todis 和 TairDB 读写 strings 性能对比



* [Todis 产品页面](https://topling.cn/products/)

* [TairDB 产品页面](https://www.alibabacloud.com/help/zh/doc-detail/145957.htm)



## 1. 什么是 Todis ？

Todis 是 Redis On ToplingDB，兼容 Redis 的分布式数据库，可以使用 Redis 客户端存取、操作数据。



## 2. 相比 TairDB，Todis 有什么优势？



Todis 的内存利用率更高，无论是读还是写数据，性能都远远超过 TairDB。

这里在 Todis 和 TairDB 读写相同原始数据的前提下，进行性能对比测试。



## 3. 全量数据对比测试结果



对于 strings 类型，测试顺序写、随机写、顺序读与随机读性能；

**在各个测试中，Todis 使用的云主机规格均低于 TairDB，具体规格在下面图示中给出。**

Strings 类型测试主要使用工具分别向 Todis 和 TairDB 中写入 3850w 条维基百科（Strings 类型）数据，然后读。

每组写测试相互独立，读测试使用写测试中写入的数据，其中随机读测试以循环读的方式取第三轮结果。

下面以条形图来显示平均速度，以折线图来显示瞬时速度。




### 3.1 平均速度

1\. 顺序写

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_seq_write_avg_speed_bar.png)

2\. 随机写

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_rand_write_avg_speed_bar.png)

3\. 顺序读

读的平均速读，取得是第二轮读的平均速度，即相对稳定后的速度。

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_seq_read_avg_speed_bar.png)

4\. 随机读

读的平均速读，取得是第二轮读的平均速度，即相对稳定后的速度。

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_rand_read_avg_speed_bar.png)







### 3.2. 瞬时速度

1\. 顺序写

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_seq_write_time_speed.png)

2\. 随机写

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_rand_write_time_speed.png)

3\. 顺序读

读的信息统计了循环读两轮的结果，即下面是读整个数据库两遍的数据。

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_seq_read_time_speed.png)

4\. 随机读

读的信息统计了循环读两轮的结果，即下面是读整个数据库两遍的数据。

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_rand_read_time_speed.png)





## 4. 热数据对比读测试结果

热数据对比测试，只对比部分数据。

测试数据规模是相对于全量数据的，全量数据约 3850w 条。

| 数据库 | 规格   | 测试数据规模（顺序读） | 测试数据规模（随机读） |
| ------ | ------ | ---------------------- | ---------------------- |
| Todis  | 4C32G  | 40%                    | 10%                    |
| Todis  | 2C16G  | 20%                    | 5%                     |
| TairDB | 16C64G | 40%                    | 10%                    |
| TairDB | 8C32G  | 20%                    | 5%                     |
| TairDB | 4C16G  | 10%                    | 3%                     |



### 4.1. 平均速度

1\. 顺序读

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_seq_read_avg_speed_percent_bar.png)

2\. 随机读

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_rand_read_avg_speed_percent_bar.png)

---

受阿里云网络带宽限制，Todis 的读写性能无法全部发挥出来。

下面是使用 strlen 进行的热数据读测试。

---

3\. 顺序读

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_seq_read_avg_speed_percent_strlen_bar.png)



4\. 随机读

![](/images/Todis-he-TairDB-读写-strings-性能对比/strings_rand_read_avg_speed_percent_strlen_bar.png)



