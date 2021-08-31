/*创建表*/
create table if not exists `d_skin`(
`id` bigint auto_increment comment '自增主键',
`user_id` bigint not null comment '玩家Id',
`card_id` bigint default 0 comment '穿戴该皮肤的卡牌ID',
`skin_type` int unsigned not null comment '皮肤类型',
PRIMARY KEY (`id`)
)comment='用户皮肤表';
/*删除表*/
drop table `d_skin`;
/*插入数据*/
insert into d_skin values(1,1,0,1);
select LAST_INSERT_ID();
select @@IDENTITY as ID;
/*删除数据*/
delete from d_skin where id =1;

/*查询数据*/
select * from d_skin where user_id = 1;
select LAST_INSERT_ID();
