/*创建表*/
create table if not exists `s_card_rank`(
`id` int unsigned comment '非自增主键,阶级',
`cost_card_num`int unsigned unique comment '升级到本阶级需要消耗同类型卡牌的数量',
`hp` bigint default 0 comment '生命值',
`mp` bigint default 0 comment '法力值',
`atk` bigint default 0 comment '攻击力',
PRIMARY KEY (`id`)
)comment='卡牌阶级属性配置表';
/*删除表*/
drop table `s_card_rank`;
/*创建卡牌池*/
begin;
insert into `s_card_rank` values(0,0,0,0,0);
insert into `s_card_rank` values(1,1,100,100,100);
insert into `s_card_rank` values(2,2,200,200,200);
insert into `s_card_rank` values(3,3,300,300,300);
insert into `s_card_rank` values(4,4,400,400,400);
insert into `s_card_rank` values(5,5,500,500,500);
insert into `s_card_rank` values(6,6,600,600,600);
insert into `s_card_rank` values(7,7,700,700,700);
insert into `s_card_rank` values(8,8,800,800,800);
insert into `s_card_rank` values(9,9,900,900,900);
commit;
/*删除*/
delete from `s_card_lev_attr`;