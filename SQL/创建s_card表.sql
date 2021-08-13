/*创建表*/
create table if not exists `s_card`(
`id` int unsigned comment '非自增主键',
`card_type` int unsigned comment '卡牌类型',
`name` VARCHAR(32) comment '卡牌名称',
`hp` bigint default 0 comment '生命值',
`mp` bigint default 0 comment '法力值',
`atk` bigint default 0 comment '攻击力',
PRIMARY KEY (`id`)
)comment='卡牌类型表';
/*删除表*/
drop table `s_card`;
/*创建卡牌池*/
insert into `s_card` values(1,1,"坦克",1500,1000,800);
insert into `s_card` values(2,2,"战士",1000,600,1700);
insert into `s_card` values(3,3,"法师",800,1700,800);
/*删除*/
delete from `s_card`;