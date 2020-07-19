# LInuxCppServer
 Linux C++ develope

致力于学习应用Cpp 进行并发模型的开发使用技术！！！


@[TOC](展现 Linux C++服务器端编程的精华 )
# 服务器开发准备阶段

## TCP/IP

**TCP/IP部分 参考笔者拙文**：[一小时通关计算机网络（ 冲冲冲！！！）](https://blog.csdn.net/chongzi_daima/article/details/106966634)


## 伯克利 socket API

server：
bind
listen
write
close

client：

connect
read
close

## 安装 Linux Ubuntu （虚拟机）
### 下载虚拟机
https://www.vmware.com/jp/products/workstation-player/workstation-player-evaluation.html

### 下载 ubuntu 系统
https://ubuntu.com/download/server
http://mirrors.melbourne.co.uk/ubuntu-releases/

### 安装 ubuntu 系统 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200622182320125.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020062218232990.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200622182342877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200622182355310.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)


![在这里插入图片描述](https://img-blog.csdnimg.cn/20200622182359677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)



![在这里插入图片描述](https://img-blog.csdnimg.cn/20200622182405350.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200622182411907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)
### XShell 
**下载**： 

https://www.netsarang.com/zh/xshell/

**使用 Xshell 连接远程服务器**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200622212505782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)

**注意：XShell 连接不上服务器的解决办法**

1. 服务器未安装 ssh 工具  或者 ssh 服务未开启


解决办法

-  检测 ssh server是否启动:ps -e | grep ssh
- 先更新软件源：  sudo apt update
-  下载ssh 工具： sudo apt-get install -y openssh-server

2. 网段不同

解决办法，在虚拟网络设置 虚拟网络8 的网关地址与 虚拟机 ip 同一网段
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200623100411809.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)
3. 重新默认配置

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200623173723339.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)

### Xftp （远程文件传输）

XFTP: XFTP 是一个远程文件传输的工具，主要用于将本地文件传输到 终端上

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200623220927334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)
## 服务器开发所用到的 Shell 命令

### 通用命令
**开机准备**：
 - `who` ： 查看现在登陆的用户信息
 - `sudo （最高权限） apt-get   update / upgrade / dist-upgrade`  : 检测系统更新/ 软件包更新 / 大版本更新
 - `sudo add-apt-respository ppa:ubuntu-toolchain-r/test`   : 更新Ubuntu环境下的最新源
 - 换网络点（提高下载速度）步骤

1. `wget -c (网点 ： http://mirrors.163.com/.help/sources.list.trusty) :` 下载网点配置
2. `cp  sources.list.trusty`   .      : 拷贝配置源文件到当前目录（/etc/apt）
3. `cd  /etc/apt         ,   cp sources.list  sources.list.bak`     备份 原配置文件
4. `cp  sources.list.trusty   sources.list` :替换原配置文件
 
 **查看**：
 
  - man （Manual） pwd  ：查看 pwd 指令的文档
   - pwd ： 查看当前目录连接
   - ls ：查看当前目录下的文件
    ls  / :查看根目录文件
    可选择： （-la  详细信息查看，a 查看其 连接计数）
    *: 代表了可查找的信息所有可替换，？ ： 代表了信息的单个替换
    - `stat aaa.txt`  :查看文件属性
    - `file  abc.txt`  :查看文件类型
    - `cat` ：输出文件内容
    - `tail  （-f， -n  + 行数）` ：查看尾部一定行数的内容
    - 

**切换目录**：
    - cd ：切换目录
    - cd / :切换根目录（绝对目录）
    - cd  (不加 /)  相对目录
    - cd  .. :返回上一目录

**创建**：

- `touch  abc.txt`   ：没有则创建文件，有则修改文件时间
- `mkdir  abc`         ：创建目录
- ln ：创建链接命令
- 

**拷贝**

- `cp   aaa.txt    aaa.bak`   :拷贝
- `cp    hzj.txt  hzj.txt`    ：替换
    -i (询问)     -l （硬连接） -s (软连接，相当于别名，跨硬盘，删除指向文件，软连接失效)
  
 


**删除**

- `rm   hzj.txt`  :删除文件（不能删除目录）
- `rmdir  hzj`  ： 删除目录（目录有文件不能被删除）
- `rm   -r   doc` : 递归删除目录（包括目录中的文件）

**修改**

- `mv hzj.txt  hzj.txt.1` :移动文件（修改文件名字）

### 查看系统上的命令

 - ps ： 输出当前用户的进程
 （-ef）（al） ：显式全部进程详细的信息
 - top ：实时显示进程信息，系统信息
 - htop ： 带图形化的 top
   下载： sudo  apt-get install htop (远程服务器下载)
  - ps  aux| grep top ：列出 top 进程的 aux 序号等信息
  - kill  -s(-q :强制结束)  INT  aut序号 ： 给该程序发送结束信号
  - df   -h  ：查看当前盘的使用情况



### 其他杂项命令

- grep (global re(正则表达式) print) ：查看服务器日志
- grep  apple（re表达式）  hzj.txt :   在文件中查找 apple 所在的行，并输出其行内容
  （-c ： 输出行   -v :输出不出现给顶表达式的行内容    -n ： 输出出现的行号与内容）
 作为管道右侧：  ps   |   grep   bash  （将 | 管道左侧的输出作为右侧语句的输入）
- zip  :压缩
- unzip ：解包
- tar
- wget  （-c :下载中断时，下次继续在此继续下载）  link（链接）
- echo   $PATH :  打印（其环境变量）
- printenv  ：输出所有环境变量
- chomd  +x （-x）  hzj.txt  :改变文件权限

- ctrl +z ：暂停任务
- jobs :查看当前任务 
- fg + 任务序号 ：恢复任务

- scp  用户名@主机：/home/handling/main.cpp  :内网主机传输文件：
 
## 简易 vim 

### vim 初学者命令

vim 默认是认为我们是只看不编辑的，所以是 命令模式 Normal

当我们键入命令，就能执行对文本的操作：


模式切换：

- ESC ：退回到 Normal 命令模式
- 命令模式 v ： Vitrual模式，移动光标选择范围，运行命令针对选项范围。
-  ： 冒号键入命令

插入命令

- i  插入光标之前
- a  插入光标之后
- o  在当前行下插入新行

移动光标：
 
 - h  ： 左移
 - j   ： 下移
 - k  ： 上移
 - L  ： 右移动

- w ： 到下一个单词开头
- e  ： 到下一个单词结尾
- ^ : 移动行首非 制表符的字符
- g_ : 移动行尾 非制表符的字符
- 0 ： 行首
- $ ; 行尾
- NG : 跳转到第 N 行
- gg ： 到第一行
- G ： 跳转文本最后一行
- % ： 光标移动到 与之匹配的括号
- *与# : 匹配当前光标所在的单词， * 是移动到下一个匹配，#是上一个

选择区块：

Visual 模式

- vi “ ： 选择两个 ”“间不包括 引号的内容， 
- va" ; 包含 ”
- vi ） ：选择 （） 内容，不包含 （）本身
- va )  : 包含 （）

ctrl + v 进行块操作，选中块，如果需要插入则 设置大写 I ，进行多行插入，之后 esc 恢复 normal 状态插入成功。


拷贝与粘贴：

- YY : 拷贝当前行
- p ： 粘贴寄存器的字符行

取消操作与重复操作：

- u  ：取消上一次操作
- ctrl + r ：重复取消操作
- . : 重复上一个改变文本的操作


查找：


-  /    查找的文本（RE） ：向下查找
- ？  查找的文本（RE） :  向上查找
- n ：跳到下一个为止， N ：跳到上一个查找位置 
- fh ： 行查找下一个字符h出现的位置，F反向
- th ： 到下一个 h 前的字符位置， T反向




删除：

- x  删除当前光标字符
- dd ：删除光标所在行
- dt“ ： 删除从光标开始的行的内容，直到遇见 “

自动补全：

Insert模式
- （ctrl + n 或 ctrl + p）：能选择匹配到的单词

自动缩进：

 - = ：选中区块自动缩进

多行连接： 

 - J ：选中行连接 


分屏操作：

- vsplit ： 分屏
- ctrl + w +w ：光标多屏跳转
- hide ：隐藏光标所在屏
- b  main.h :同时编辑文件时将光标所在屏转到 main.h

 


退出： 

- wq ： 保存退出
- q！：强制退，不存盘
- ZZ : 存盘退出

环境变量：

：set  hlsearch ：高亮查找
：set number : 显式行号

帮助

- help ：vim 使用规则 
- vimtutor :专门介绍 vim 的使用规则

### vim 配置文件与环境变量


vim 环境变量配置在用户目录的 .vimrc中

环境变量配置：


# 服务器开发初始阶段
## 深入学习 C++
### 准备： 配置环境
Gcc下载： sudo apt-get install g++
make : 工程管理
cmake ：更高的工程管理

### 关键字：KEY-WORDS

**alignas 与 alignof （c++11）：**

alignas() 是能够显式设置其对齐字节数的。//默认设置为  最大成员字节数为对齐字节数
这种明确规定占用字节大小后，编写代码将更具有跨平台性。

而 alignof 是查看当前对象的  对齐字节数

```cpp
struct alignas(4) A {
  int a;
  int b;
};
```

class 与 strcut 采用字节对齐来安排类内的内存，默认以最大的成员字节为 对齐字节数，未满均以对齐字节数的额外差值补齐。

**bool：（平台不同影响不同）**

建议： 
  
  - 作为条件表达式，使用自身作为条件，而非  bool == false 。。。
  - bool  不要作为函数参数，因为传递的过程是非常难以理解的，使用  enum class 来替换。

**char** 

在linux平台下是 -128  - 128 （signed char）
在其他平台下  0 - 256    （unsigned  char）

**constexpr : 常量表达式** 
可以作为函数返回值，或是其他变量等等（只要给定情况下，能在编译期间求值的话，均不会在运行期间求值）

**const_cast**

出现 const_cast 的情况即代表代码结构本身有问题（打破了正常的规则，对不变的性质做出改变）
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200625080805278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)

**nullptr （c++11）与 NULL和 0 的区别与联系**

nullptr 是作为空指针类型进行表示的，类型是（std::nullptr_t）,目的是在类型推断时避免 使用（NULL 和  0 推断结果的异样）。

- 0  推断为 int类型
- NULL 是宏定义，在 c 语言是 （void*）0， 在 c++中 是 0LL

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200625094133476.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nob25nemlfZGFpbWE=,size_16,color_FFFFFF,t_70)
或者

```cpp
auto  p1 = 0;
decltype(p1) p2;  // p 是int类型

auto p1 = NULL;
decltpe(p1) p2; // p2 是 longlong类型

auto p1 = nullptr;
decltype(p1) p2; // p2 是指针类型：std::nullptr_t
```

**(static_cast)    (const_cast )  (reinterpret_cast)   (dynamic_cast )   (c like cast) 五种类型转换的区别**

四种类型各尽其职务，互相独立，操作之间不会存在相同规则


static_cast : 显式地进行 符合c++程序的 所有隐式转换规则，如 非const 转换为 const， int ->dobule ,可能会损失精度，当然也不能进行 const_cast  操作了，非隐式转换

const_cast : 只能改变运算对象的底层 const， 出现该 类型转换则违背了一定的不进行改变 的规则，需要检查代码是否合理

reinterpret_cast : 它可以无视种族隔离，随便搞。但就像生物的准则，不符合自然规律的随意杂交只会得到不能长久生存的物种。随意在不同类型之间使用reinterpret_cast，也会造成程序的破坏和不能使用。不能进行 const_cast  操作

dynamic_cast ：用于将基类的指针或引用安全地转换为派生类的指针或引用。

<br>

**static_assert (c++11)**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200625104103479.png)
编译时发现条件不满足， 输出 msg， 对一些行为的安全处理与警告信息的传递



<br>
<br>
<br>

### OOP 

**前置声明：**

- 使用前置声明，能避免在头文件定义时使用其他头文件，额外地对 “两个类之间的先后存在”完美地解释。
- 除非必须引用头文件达到现文件的实现（若在源文件实现，在源文件中引入），否则尽可能少引用，会减缓编译器编译速度

一般来说：

1. 当实现一个目的时，需要清楚地明白引入的东西的大小或者里面有的东西，这时务必引入（作为基类，类成员或者标准库模板参数）
2. 当作为 返回值，接口等等 使用前置声明即可


**三五原则**

在原则上一定要考虑好 自赋值与赋值前内存的管理问题

**assert 预处理宏**
assrt( expr) ：我们使用 assert 处理一些行为上必须正确的事情。
对expr求值，如果表达式为假，assert 输出信息并终止程序的执行，如果表达式为真，则assert什么都不做


**析构函数不要被默认合成**

析构函数默认是 内联且 noexpect，因为内联展开的析构函数是非常庞大的，因此我们要阻止该合成方式。
内部声明，外部定义且非内联。

**析构函数不能抛出异常**

析构函数默认是 noexcept，抛出异常，会直接导致程序崩溃， std::terminal
如果我们强制 noexcept(false), 会在程序进行中出现很多不必要的问题。


**鼓励构造函数抛出异常**

构造函数结束后对象一定要是符合要求的，所以抛出异常是对要求的安全处理，避免后来使用造成不必要的问题。


**vitrual 虚函数声明**
 
  vitrual 声明要权衡，因为 声明为 virtual 编译器就会对其做额外的事情。



## 偏僻问题

### 1. 如何判定一个类中是否有成员？

答：

-  引入头文件 <type_traits>

#include <type_traits>

```cpp
class A {
 private:
  size_t  sz_;
};
int main() {
   std::cout<< std::is_empty<A>();
}
```

- 创建带有内存成员的派生类模板继承于 该类，之后对比其派生类内存是否与派生类的成员（不包含基类成员）内存之和相同。

注意： 空类的 sizeof  是  1 ，在struct 或 class 中，默认拥有对齐字节（alignof）数，为最大的子元素的字节大小， 假设该 alignof 为 4，那么该struct 的 sizeof 等于   ：指定子成员类型前后成员所占总字节（n/4+ n%4 ==0 ? 0: 1）和，加上所有指定子成员类型所占字节数，空类与 只有一个 1 个字节的成员的类 内存相同。

```cpp

class A {

};

template<typename T>
class TempClass : T {
  char data[10];
};

template<typename T>
bool IsEmptyClass(const T &);

template<typename T>
bool IsEmptyClass(const T &obj) {
  return sizeof(TempClass<T>) == sizeof(char[10]);
}



```


### 2. sizeof 对于 类/结构体 的尺寸估测

```cpp
class A {
 private:

};
int main() {


   std::cout << sizeof(A);
}

```

- 如果是空成员类，则类在内存中保留了一个 字节的占位
- 非空成员类，则类大小等于 

```cpp
struct A {
  char a;
  char d;
  int b;
  char c;
};
```
- 在struct 或 class 中，默认拥有对齐字节（alignof）数，为最大的子元素的字节大小， 假设该 alignof 为 4，那么该struct 的 sizeof 等于   ，指定子成员类型前后成员所占总字节（n/4+ n%4 ==0 ? 0: 1）和，加上所有指定子成员类型所占字节数。
### 3. 当传递多动态内存参数时，拒绝在参数传递时就地开辟内存

```cpp
class A {
 public:
  A() { throw runtime_error(""); }
};
class B {

};
void print(A *a, B *b) {
  delete a;
  delete b;
}
int main() {
  // 可能会出现 A 与 B  同时分配内存，当A调用构造函数时抛出异常，
  // A的析构未完成（构造失败），A 的内存会被系统回收
  // B 的资源就会消失了
  print(new A(), new B());

}
```

建议： 尽量以单线程的方式，顺序进行构造完毕后传入指针实参。

### 4.  模拟 扩容（预先足够分配内存）的类对象的动态内存管理

我们利用 定位 new 表达式，在指定（足够内存的地址）分配类对象，析构时只进行析构，不进行回收内存

```cpp

class Object {
 public:
  Object() :p_value_(new int(4)){}
  ~Object() { delete p_value_; }
 private:
  int    data_[100];
  int   *p_value_;
};

char info[10000];
int main() {
  //定位new 表达式，只在info 构造，不开辟内存
  Object *s = new(info)Object();
 
  s->~Object();  //显式析构，不释放内存

 // delete  s;  错误，释放了未被构造的内存（多管闲事）


```

### 5. 移位超限与进制转换

移位操作：

```cpp
int a = 0xFF
int b =  a << 33; //超出所能表达的位数 

与上面等价

int b = a << 1;   (a << n   == a << (n % a所表达的位数))

```

16 进制转换：

0x ：代表前缀（标识作用）
ABCDEF: 10 -15

OxFF  =  F * 16^0 + F * 16 ^1 

