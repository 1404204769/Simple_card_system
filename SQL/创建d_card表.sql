/*创建表*/
create table if not exists `d_card`(
`id` bigint auto_increment comment '自增主键',
`user_id` bigint not null comment '玩家Id',
`card_type` int unsigned not null comment '卡牌类型',
`name` varchar(32) comment '卡牌名',
`exp` bigint default 0 comment '经验值',
`lev` int unsigned default 0 comment '等级',
PRIMARY KEY (`id`)
)comment='用户卡牌库';
/*删除表*/
drop table `d_card`;
/*插入数据*/
insert into d_card(user_id,card_type,name,exp,lev)values(1,1,"",0,0);
insert into d_card values(0,1,1,"",0,0);
select @@IDENTITY as ID;
/*删除数据*/
delete from d_card where id =1;
/*更新数据*/
update d_card set user_id=2,name="test",exp=1,lev=1 where id = 3;

/*查询数据*/
select * from d_card where user_id = 1;
select LAST_INSERT_ID();
