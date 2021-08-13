/*创建表*/
create table if not exists `d_test`(
`id` bigint auto_increment comment '自增主键',
`account` VARCHAR(16) not null unique comment '登入账号名,唯一键',
`create_time` timestamp default current_timestamp() comment '创建时间',
PRIMARY KEY (`id`)
)comment='账户时间表';
/*删除表*/
drop table `d_test`;
/*录入操作：输入account保存到数据库中*/
/*使用Inset into 表名(字段1,字段2) values(值1,值2)进行插入,并对查看插入数据是否成功*/
/*推荐使用的原因:这一次我们设置了没有给id赋任何值包括null,而且不用关心表中字段的顺序,比如下面不按照正常顺序添加,我们将account放在第一,create_time放在第二个.也能添加成功;需要注意的是表名后面的字段名必须和后面values赋的值保持一致;实际开发中在维护和扩张方面都比方案一要好.*/
insert into `d_test`(account) values ("李志诚");
insert into `d_test` values (0,"塞2拉",NOW());

/*replace insert 此语句的作用是当我们在插入一条数据时，如果此条已经存在，那么先删除原来存在的数据再添加插入的数据，
如果不存在那么直接插入新的数据。注意：区分是否存在是通过主键来确定的*/
replace into `d_test`(id,account,create_time) values (1,"00000000001",now());
/*
> Affected rows: 2
> 时间: 0.007s
*/
/*INSERT IGNORE INTO 语句
此语句的作用是如果插入的数据已经存在那么就忽略插入的数据（也就是不改变原来的数据），如果不存在则插入新的数据。
注意：区分是否存在是通过主键来确定的*/
insert ignore into d_test(id,account) values (1,"00000000002");
/*
> Affected rows: 0
> 时间: 0.001s
*/
/*查询操作，输入account，从数据库中查询该account的创建时间*/
select create_time from d_test where account="00000000001";
/*查看表项用到的字符集防止产生中文乱码现象*/
show create table d_test;

/*
修改语句
*/
update d_test set account='xxx' where account='lzc'


select * from d_test where id='8';
update d_test set account='456' where account='123';
update d_test set id=1 where id='7';
delete from d_test where Account= '132';


/*
SQL注入问题
1.'union select user(),database()#；
*/

select * from d_test where account=''or 1=1#';

select * from d_test where account='\' or 1=1#';

select * from d_test where Account='\' or 1=1';

select * from d_test where account='\' or 1=1';
