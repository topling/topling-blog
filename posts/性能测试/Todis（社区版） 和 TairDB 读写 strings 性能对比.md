这篇文章对比的是 Todis 社区版和 TairDB。

Todis 社区版相比与企业版，性能略低，  
在 [Todis 和 TairDB 读写 strings 性能对比](https://blog.topling.cn/posts/Todis%20%E5%92%8C%20TairDB%20%E8%AF%BB%E5%86%99%20strings%20%E6%80%A7%E8%83%BD%E5%AF%B9%E6%AF%94/) 这篇文章中查看 Todis 企业版与 Todis 的性能对比。

这篇文章删除了部分解释说明文字，更详细内容同样查看上面的文章。

# 1. 测试数据：Wikipedia 英文版
总尺寸	| 109 GB
--------|--------
总条数	| 3850 万条
平均长度|	2.8 KB

# 2. 全量数据对比测试结果

**在各个测试中，Todis 使用的云主机规格均低于 Tair，具体规格在下面图示中给出。**

对于 strings 类型，测试顺序写、随机写、顺序读与随机读性能。
* 写数据使用多线程并发 mset，每次写 32 条数据，相比 set 每次写一条数据，减小了网络往返开销
* 读数据使用多线程并发 mget，每次读 32 条数据，相比 get 每次读一条数据，减小了网络往返开销
* 写完数据之后，读取数据之前，不执行手动 compact，这样更接近真实的业务场景

Strings 类型测试使用工具分别向 Todis 和 Tair 中写入测试数据（Strings 类型）数据，然后读。

每组写测试相互独立，读测试使用写测试中写入的数据，其中随机读测试以循环读的方式取第三轮结果。

下面以条形图来显示平均速度，以折线图来显示瞬时速度。

## 2.1 平均速度

平均速度的条形图简单直观，可以从中清楚地看到对比结果。

### 2.1.1 **写数据**

先看 **顺序写**：顺序写的写放大较低，所以性能更高
![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_seq_write_avg_speed_bar.png)

再看 **随机写**：随机写的写放大较低，所以性能较低
![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_rand_write_avg_speed_bar.png)

### 2.1.2 **读数据**


`读数据用的是 mget，取的是第二轮读的平均速度，即相对稳定后的速度`

先看 **顺序读**：顺序读缓存命中率极高，所以性能都很好

![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_seq_read_avg_speed_bar.png)

再看 **随机读**：随机读缓存命中率很低，所以性能都比顺序读要差很多

![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_rand_read_avg_speed_bar.png)

## 2.2. 瞬时速度

每个时刻的瞬时速度，我们用折线图来表达，其坐标横轴是时间，纵轴是瞬时速度。因为读写的数据量相同，Todis 速度快，很快就结束了，所以 Todis 横轴长度很短，Tair 的横轴长度很长。

### 2.2.1 写数据
写数据 Todis 和 Tair 都有是先快后慢，并且有一定的波动，其中低规格 Tair 波动的周期性非常明显。

先看 **顺序写**

![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_seq_write_time_speed.png)

再看 **随机写**

![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_rand_write_time_speed.png)

### 2.2.2 **读数据**

读数据用的是 mget，记录了循环读两轮的结果，即下面是读整个数据库两遍的数据。

* Todis 在读取测试之前清空了操作系统 Cache，从而相当于是冷启动的，并且关闭了操作系统预读(madvise random)
* Tair 没有提供清空 Cache 的命令，也无法登录 Tair 机器去清空 Cache，相当于是热启动的，合理猜测 Tair 应该也关闭了预读

先看 **顺序读**

![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_seq_read_time_speed.png)

再看 **随机读**


![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_rand_read_time_speed.png)

# 3. 热数据对比读测试结果

在真实的业务场景中，数据有冷热之分，这也是我们之所以选取 Todis 或 Tair 的理由，在该测试中，我们将一部分数据作为热数据进行测试，下表中热数据占比是相对于全量数据的百分比：

| 数据库  | 规格   | 热数据占比（顺序读） | 热数据占比（随机读） |
| ------ | ------ | ------------------:| ----:|
| Todis  | 8C64G  | 40%                | 10%  |
| Todis  | 4C32G  | 20%                | 5%  |
| Todis  | 2C16G  | 10%                |  3%  |
| Tair | 16C64G | 40%                | 10%  |
| Tair | 8C32G  | 20%                |  5%  |
| Tair | 4C16G  | 10%                |  3%  |

## 3.1 平均速度

先看 **顺序读**

![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_seq_read_avg_speed_percent_bar.png)

再看 **随机读**

![](https://raw.githubusercontent.com/topling/topling-blog/main/resource/性能测试/Todis（社区版）%20和%20TairDB%20读写%20strings%20性能对比/strings_rand_read_avg_speed_percent_bar.png)
