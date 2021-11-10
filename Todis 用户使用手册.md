# Todis 用户使用文档

## 1. 注册登录
访问[注册页面](https://console.topling.cn/register) 按照提示填写所需信息。

内测阶段需要邀请码完成注册。
<!--more-->

![注册页面](/images/topling-usage-doc/signup.jpg)

填写完成后，将收到邮箱确认邮件，**确认完毕**后转到[登录页面](https://console.topling.cn) 填写用户名与密码登录。





## 2. 拓扑岭辅助工具

鉴于并网过程比较复杂，这里提供了简单的辅助工具，用户可以填写阿里云的 AccessId 以及 AccessSecret 来一键完成。

**本程序不会收集或上传任何您的个人信息**，也请不要从第三方下载辅助程序—— AccessId 和 AccessSecre 重要性等同于您的阿里云账号以及密码。

阿里云的用户 ID、AccessId 和 AccessSecret 可以登录阿里云账号获取。

[点击这里获取辅助工具](https://topling.cn/downloads/)

[所有所有版本以及源码地址](https://github.com/topling/topling-alpha-test-helper/releases)

![](/images/topling-usage-doc/download-page.png)

可以根据自身环境选择可执行程序，安装过 .NET 5运行时的用户可以选择不带 no-deps 的程序，体积较小；带有no-deps的程序不需要安装运行时，体积较大。

程序运行界面如下图所示：

![tools](/images/topling-usage-doc/topling-helper.png)

填写对应项，并点击**提交**按钮，等待几分钟即可。

如果运行失败，再检查填写无误后 (发现错误可以直接修正后点击提交，不影响结果)，重复运行也不会影响执行结果。

[点击这里获取阿里云 AccessId 和 AccessSecret](https://ram.console.aliyun.com/manage)

![access](/images/topling-usage-doc/access.png)

点击**创建 AccessId**， 会创建出新的 AccessId，将填写到工具对应位置

**请保护好您的AccessId 和 AccessSecret，不要将它们泄露给其他人**

**请保护好您的AccessId 和 AccessSecret，不要将它们泄露给其他人**

![](/images/topling-usage-doc/secret.png)

在阿里云控制台右上角点击头像， 获取阿里云 ID。

![](/images/topling-usage-doc/aliyun-id.png)

阿里云的用户 ID、AccessId 和 AccessSecret 以及拓扑岭账号等对应数据填入辅助工具并点击"提交"即可

![tools](/images/topling-usage-doc/topling-helper.png)


使用辅助工具完成操作后，可以直接[开始使用数据库](#use-todis)


## 3. 预留网段
使用一键工具的用户可以跳过本节，直接[开始使用数据库](#use-todis)

由于我们的数据库服务器 (ECS) 需要阿里云来承载，而数据库服务器用户服务器运行在不同的账号下，因此需要预留承载我们服务器的网段地址，就如同阿里云创建 ECS 前必须先创建 VPC 一般。

预留创建完成后，就可以在预留网段上创建服务了。

### 3.1. <span id="create-vpc">创建预留网段</span>

进入后台，主页右上角能看到 “创建新网段” 的字样，创建网段后，
将在我们的账号下为您预留一段地址，我们的实例将在这段地址下运行。
点击创建新网段，弹出创建窗口。
![创建预留网段](/images/topling-usage-doc/create-vpc.png)

选择服务商与区域，由于**数据库使用内网进行连接**，
请选择准备试用地址相同的服务商与地域。当前内测阶段仅提供阿里云 - 成都。
点击**确认**并等待数秒创建完成。
![创建预留网段窗口](/images/topling-usage-doc/create-vpc-console.png)

我们的预留地址空间默认为自动生成的随机值，请注意**该地址不能和您准备试用产品的 VPC 中包含的交换机地址相同，否则后面将并网失败**，
您也可以取消自动生成，填写一个不会占用的地址作为预留地址空间。

创建完成后，预留网段列表中将出现刚刚创建的网段。
![表格](/images/topling-usage-doc/vpc-table.png)

### 3.2. 并网

#### 3.2.1. <span id="create-cen">创建云企业网</span>
如果您已经在成都有可用的云企业网，可以跳过本节，直接开始[加入云企业网](#join) 步骤。

关于云企业网的详情，可以参考帮助文档[什么是云企业网](https://help.aliyun.com/document_detail/59870.html)。

进入[您的阿里云云企业网页面](https://cen.console.aliyun.com/cen/list)，选择创建云企业网，右侧会弹出创建表单。

![创建云企业网](/images/topling-usage-doc/create-cen.png)

按照要求填写，并加入您现有的 VPC——内测阶段仅支持阿里云 - 成都，请加入一个成都的 VPC 实例。如果您在成都没有 VPC，请[在成都创建一个新的 VPC 实例](https://vpc.console.aliyun.com/vpc/cn-chengdu/vpcs/new)。

为防止并网失败，建议使用 B 类地址 (172 开头) 或 C 类地址(192 开头)。

**注意您的 VPC 下交换机不要和我们的预留网段地址重叠，否则将并网失败！**


填写内容类似下图:

![创建云企业网 - 填写](/images/topling-usage-doc/create-cen-content.png)

创建完成后，可以看到我们刚刚创建的云企业网:
![新云企业网](/images/topling-usage-doc/new-cen.png)

记录刚刚创建的云企业网 id 与您的阿里云 id，为后面并网做准备。

您的阿里云 id 可以在[阿里云控制台](https://console.aliyun.com) 中鼠标移动到头像上获取(不需要点击)，应为一个数字。
![阿里云 id](/images/topling-usage-doc/aliyun-id.png)

#### 3.2.2. <span id="join">加入云企业网</span>

点击上面创建的网段所在行中的齿轮图样，将弹出操作页面。
![edit-vpc](/images/topling-usage-doc/edit-vpc.png)

由于我们没有进行过并网，因此后两个选项不可选。

暂时忽略后两项，点击确定，进入并网操作
![操作 VPC](/images/topling-usage-doc/vpc-operation.png)

在弹出的窗口中选择填写您[上一步](#create-cen) 创建的云企业网以及您的阿里云 id。目的是让我们的预留网段 VPC 对您的云企业网授权。

![授权云企业网](/images/topling-usage-doc/join-cen.png)

填写完成后，进入[阿里云云企业网管理页面](https://cen.console.aliyun.com/cen)，以加入我们的预留网段。
此时应看到您的云企业网列表。

![新云企业网](/images/topling-usage-doc/new-cen.png)

在刚刚加入的云企业网实例上，点击 "管理"，进入页面，如下图:

![加入](/images/topling-usage-doc/aliyun-join-cen.png)

点击 "加载网络实例"，粘贴我们的 id(1343819498686551)，实例类型选择 VPC，地域选择预留网段的地域 (内测阶段仅有成都)，页面上将出现可以选择的 VPC 实例 (即刚刚授权的 VPC)。如下图，点击 "确定" 后，我们预留网段将和您已经加载的 VPC 互通。

![带内容的加入](/images/topling-usage-doc/join-cen-content.png)

如果加入失败，则可能是您的现有交换机地址和预留网段冲突，请[重新创建预留网段](#create-vpc)，每次创建默认会创建随机的预留地址，也可以取消随机生成，并手动选择一个您认为不会冲突的 / 24 地址。

**正确完成并网的云企业网下至少有两个VPC！**


## 4. <span id="create-vpc">实例 (数据库) 相关</span>

使用一键工具的用户可以跳过本节，直接[开始使用数据库](#use-todis)

### 4.1. 创建实例

在准备创建实例的预留网段上点击 "创建实例"。选择可用区，实例类型以及数据库类型。点击 "确定" 后等待数秒，数据库开始初始化。
![](/images/topling-usage-doc/create-instance-window.png)
数据库初始化时间约为一分钟，刷新窗口后可以得到地址，注意使用 10 开头的私网地址进行连接 (数据库不提供公网连接功能)。

内侧阶段最多能创建一个实例。

### 4.2. 删除实例

在实例列表上点击 "删除"，即可删除实例。注意实例删除后不可恢复。

## 5. <span id="use-todis">使用产品</span>
使用一键工具的用户可以从本节开始

在您已经加入云企业网的 VPC 上创建一个 ECS(已有可跳过)，可以使用 redis-cli 等客户端 / sdk 连接 (注意使用私网地址进行连接)。如果连接不通 (包括 ping)。则可能并网没有成功，请确认您的云企业网中包含了待测试的 ECS 所在 VPC 以及我们的预留网段的 VPC。

### 5.1. 监控
点击**实例中的 grafana**以及**引擎监测**可以查看对应的实例监控。
![监控](/images/topling-usage-doc/monitor-instance.png)

### 5.2. 测试性能
我们上传了一些测试工具与数据集供使用 Centos 的阿里云用户使用。

[测试下载页面](https://topling.cn/downloads/)

准备工作

``` bash
#下载自动脚本
wget https://topling.cn/downloads/mount-test.sh && chmod +x ./mount-test.sh && ./mount-test.sh /mnt
yum install zstd -y ## 需要 zstd 解压
#挂载测试程序及源数据到 /mnt
mount-test.sh /mnt
```

``` bash
## 直接执行查看帮助
/mnt/InsertKeys
## 插入顺序数据(源文件过大，已使用 zstd 压缩)
zstd -d -c -q /mnt/wikipedia-flat-seq.zst  | /mnt/InsertKeys -h <todis ip> -t 8 --multi-set 32 -f /dev/stdin
## 插入乱序数据(源文件过大，已使用 zstd 压缩)
zstd -d -c -q /mnt/wikipedia-flat-rand.zst | /mnt/InsertKeys -h <todis ip> -t 8 --multi-set 32 -f /dev/stdin
## 读取数据(顺序)
/mnt/GetKeysQps -t 64 -n 32 -f /mnt/wikipedia-flat-key-seq.txt
## 读取数据(乱序)
/mnt/GetKeysQps -t 64 -n 32 -f /mnt/wikipedia-flat-key-rand.txt
```

注意数据库暂不支持 flushdb 在内的大部分管理命令以及 lua 脚本。


## 6. Q&A

### 6.1. 辅助工具报错

请确认您在工具中填写的数据是否正确。如果无误，那么可以尝试重复执行 (辅助工具执行效果是幂等的)。

另外，请保证您的账号成都下的云企业网数量不超过 4，阿里云的 VPC 数量不超过 9，否则会因为阿里云的限制而导致使用失败。

### 6.2. 并网失败

请检查预留网段和您创建的 VPC 中的交换机是否重叠。

为了防止并网失败，您自己的 VPC 请使用 B 类私网地址 (172 开头) 或 C 类私网地址(192 开头)，因为拓扑岭预留网段使用 10 开头的 A 类私网地址。

### 6.3. 无法通信

请检查云企业网下是否有您在我们后台创建的 VPC。

**如果并网成功，您的云企业网下面应该挂载了至少两个 VPC。**

### 6.4. 需要和不止一个 VPC 通信
在自己的云企业网上加入对应的即可，无需在拓扑岭后台进行修改。

### 6.5. 连接会断开 / 写入失败 / 写入极慢 (KB 级别)

并网的实例是否同可用区 (目前为成都)，这一点非常重要！

### 6.6. 运行一段时先后无法写入

产品为分布式执行，暂时内测阶段暂时不保证运行环境兼容，如果我们后台升级了运算服务器的版本，
可能会导致旧版本数据库程序无法写入。
删除数据库实例并创建新数据库后可恢复。
