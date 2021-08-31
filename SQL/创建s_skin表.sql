/*创建表*/
create table if not exists `s_skin`(
`id` int unsigned comment '非自增主键',
`name` VARCHAR(32) comment '皮肤名称',
`hp` bigint default 0 comment '生命值',
`mp` bigint default 0 comment '法力值',
`atk` bigint default 0 comment '攻击力',
PRIMARY KEY (`id`)
)comment='皮肤类型表';
/*删除表*/
drop table `s_skin`;
/*创建卡牌池*/
insert into `s_skin` values(1,"普通",100,50,10);
insert into `s_skin` values(2,"高级",200,100,20);
insert into `s_skin` values(3,"稀有",400,200,40);
insert into `s_skin` values(4,"史诗",800,400,80);
insert into `s_skin` values(5,"传说",1600,800,160);
insert into `s_skin` values(6,"神话",3200,1600,320);
/*删除*/
delete from `s_skin`;