/*创建表*/
create table if not exists `s_equip`(
`id` int unsigned comment '非自增主键',
`name` VARCHAR(32) comment '装备名称',
`pos` int unsigned comment '装备位置',
`hp` bigint default 0 comment '生命值',
`mp` bigint default 0 comment '法力值',
`atk` bigint default 0 comment '攻击力',
PRIMARY KEY (`id`)
)comment='装备类型表';
/*删除表*/
drop table `s_equip`;
/*创建装备池*/
insert into `s_equip` values(1,"头盔",1,50,30,10);
insert into `s_equip` values(2,"护额",1,10,20,10);
insert into `s_equip` values(3,"护心镜",2,50,30,10);
insert into `s_equip` values(4,"防弹衣",2,10,20,10);
insert into `s_equip` values(5,"布靴",3,50,30,10);
insert into `s_equip` values(6,"凉鞋",3,10,20,10);
/*删除*/
delete from `s_equip`;

select pos from `s_equip` where enum_col =0;

update s_equip set pos=1 where id=1;