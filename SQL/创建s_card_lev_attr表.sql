/*创建表*/
create table if not exists `s_card_lev_attr`(
`id` int unsigned comment '非自增主键',
`lev`int unsigned unique comment '卡牌等级，唯一键',
`hp` bigint default 0 comment '生命值',
`mp` bigint default 0 comment '法力值',
`atk` bigint default 0 comment '攻击力',
PRIMARY KEY (`id`)
)comment='卡牌等级属性配置表';
/*删除表*/
drop table `s_card_lev_attr`;
/*创建卡牌池*/
begin;
insert into `s_card_lev_attr` values(1,1,100,100,100);
insert into `s_card_lev_attr` values(2,2,200,200,200);
insert into `s_card_lev_attr` values(3,3,300,300,300);
insert into `s_card_lev_attr` values(4,4,400,400,400);
insert into `s_card_lev_attr` values(5,5,500,500,500);
insert into `s_card_lev_attr` values(6,6,600,600,600);
insert into `s_card_lev_attr` values(7,7,700,700,700);
insert into `s_card_lev_attr` values(8,8,800,800,800);
insert into `s_card_lev_attr` values(9,9,900,900,900);
commit;
/*删除*/
delete from `s_card_lev_attr`;