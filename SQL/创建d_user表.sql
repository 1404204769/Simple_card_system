/*创建表*/
create table if not exists `d_user`(
`id` bigint auto_increment comment '自增主键',
`account` VARCHAR(16) not null unique comment '登入账号名,唯一键',
`create_time` timestamp default current_timestamp() comment '创建时间',
`name` varchar(32) comment '玩家名',
`exp` bigint default 0 comment '经验值',
`lev` int unsigned default 0 comment '等级',
PRIMARY KEY (`id`)
)comment='账户时间表';
/*删除表*/
drop table `d_user`;

insert into `d_user`(account,name) values ("李志诚","master");

insert into `d_user`(account,name) values ("lzc","master");


update `d_user` set `name`='master';

update `d_user` set account='ls',`name`='主人',exp=2,lev=2 where id=1;

select * from `d_user`;

explain select * from `d_user` where `name`='李志诚';
select * from `d_user`;
select * from d_user where account = '123';