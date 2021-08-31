create table if not exists `d_equip`(
`id` bigint auto_increment comment '自增主键',
`user_id` bigint not null comment '持有装备的用户的ID',
`equip_type` int unsigned not null comment '此装备的类型',
primary key(`id`)
)comment='用户装备表';

drop table `d_equip`;

insert into d_equip values(0,1,1);

delete from d_equip where id=1;