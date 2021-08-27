# Simple_card_system
```txt
这是一个使用C++实现的简单卡牌系统
此系统优化了上一个作品（Simple_player_system）的代码，降低了各个层次之间的耦合度，提高了代码的健壮性
```
## 1.0版本

### 实现以下功能：

* 用户登入(内含注册)注销功能
* 为指定玩家增加指定类型的卡牌
* 为指定玩家删除指定卡牌
* 查询指定玩家的所有卡牌信息
* 查询指定玩家指定卡牌的攻击力
* 查询所有玩家信息

> 逻辑：用户卡牌增删改查逻辑
>
> 卡牌实体化时为0 ，此时不管对他做什么操作都是允许的
>
> 当新增卡牌mark被标注为1时，更改操作不需要修改mark，删除操作修改mark为2，查询操作用不到
>
> 当删除卡牌mark被标注为2时，不允许重名，不允许修改，不允许查询
>
> 当更新卡牌mark被标注为3时，无法被标记为1、4
>
> 当查询卡牌mark被标记为4时，可以对其做任何操作

### 内部结构划分如下
1. 业务层
	* [LoginSystem](内部封装了用户登入登出的接口)
	* [LevelSystem](内部封装了用户升级的接口)
	* [CardSystem](内部封装了1.给指定用户增加指定类型卡牌2.给指定用户删除指定卡牌3.显示指定用户拥有的所有卡牌信息4.显示指定用户指定卡牌的攻击力)
2. 数据管理层
	+ [UserMgr](向上提供了显示所有用户信息，登入用户，注销用户，获取用户信息，检查是否注册等接口)
	+ [CardTypeMgr](向上提供了初始化，获取指定类型卡牌数据，显示所有类型卡牌信息的接口)
	+ [CardMgr](向上提供了对卡牌的增删显示的接口)
3. 数据层
	+ [User](对应数据库内的d_user表,内部向下封装对数据库的应用,在析构的时候调用,向上提供了获取本身数据的接口)
	+ [CardType](对应数据库内的s_card表,是在程序运行前就配置好的信息,程序运行时不允许求改,但允许读取)
	+ [Card](对应数据库中的d_card表,是一个动态存储表,使用mark来标记当前的状态,1表示当前卡牌是新增加的2表示当前卡牌是要删除的3表示当前卡牌被改动了,是需要更新的4表示当前卡牌刚刚从数据库中加载进来无需启动,在析构的时候根据mark的值调用对应的函数)
4. 数据库层
	* [DB](由DB类封装了对数据库的增删改查工作，是一个通用的接口，用户只需要传入包含处理好的SQL语句的Query实体即可)


## 1.1修正版

> 在CardMgr内以CardId作为key来存储数据，新建Card直接插入数据库，缓存机制只对删除与修改生效，新建一个删除卡牌堆，每次删除的时候就将数据移交给删除堆，数据堆只存放有效数据，mark标记更改为bool类型，初始为false，表示未变化，当发生变化的时候置为true，析构的时候调用update

## 1.2修正版

> 简化了Query连接的开销，在DB类中维护一个Query指针，获取Query连接的时候判断此指针是否有效，无效则新建，有效则直接返回

## 1.3修正版


>* ***将Card初始化函数Init拆分为两个个方法(数据库加载卡牌、新建卡牌) ***
>* *** 将vector存放待删除的数据最后一起删除改为直接删 ***
>* *** 传参除了指针要判断外，其余参数也要判断是否合法***
>* *** 代码尽量一行解决，控制台打印输出封装函数 ***
>* *** 使用智能指针unique防止新建指针忘记释放 ***
>* *** 重命名的类型改为CardMap这样简洁的名字 ***
>* *** CardMgr 上增加一个对应User的指针 ***
>* *** 变量能使用const的尽量使用const *** 
>* *** 默认值没必要用传参的方式 ***
>* *** 函数返回指针前都判断一次  ***
>* *** 将组合使用的函数合成一个 ***
>* *** 尽量将指针改为引用 ***
>* *** 代码根据逻辑分段 ***
>* *** 函数命名简洁化 ***
>* *** 学习使用assert ***


## 2.0版本

>增加了卡牌等级的相关内容  
>数据库新增一张静态卡牌等级表  
>重写了Card类的初始化函数，新增了卡牌等级配置类  
>新增了卡牌升级命令，修复了卡牌攻击力查询命令  

## 2.1修正版
>修复了卡牌等级超出上限导致连接错误引发的Query连接损坏引起的数据库连接失效
>修复了升级负数等级还能成功的bug

## 3.0版本
>增加了皮肤插件
>数组库新增皮肤配置信息、玩家皮肤数据
>增加了获取皮肤、穿上皮肤、脱下皮肤三个功能

## 3.1修正版
>修复了皮肤穿戴控制数据的同步问题(将双属性控制改为单属性)
>减少了if的层次
>修正了Card表的设计，去除了新增的skin_id字段，在Skin表内增加了card_id字段

## 4.0版本
>增加了升阶系统，可以消耗同种类型卡牌进行升阶，升阶后获得更强的属性
>数组库新增升阶配置信息、卡牌阶级属性
>增加了升阶功能

## 5.0版本
>增加了装备系统，每个卡牌都有自己的装备位，每个用户有自己的装备库

## 5.1修正版
>修复了一件装备可以被多张卡牌同时穿戴的bug
>修复了指定查询卡牌攻击力和显示所有卡牌信息中攻击力不相等的bug
>修复了静态配置的初始化失败导致的一系列bug
>修复了指针移交有可能失败导致内存泄漏的bug
>优化了装备位(使用map存储)  （使用常量记录装备位大小）
>使用循环操作装备位相关业务
>wear函数的传参直接传入equiptype指针,防止无效参数
>装备脱穿函数移到cardsystem实现，降低耦合度
>使用静态配置指针失败的情况