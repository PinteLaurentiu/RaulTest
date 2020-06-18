create schema images collate utf8_general_ci;

create table image
(
    id bigint auto_increment primary key,
    width int not null,
    height int not null,
    imageData longblob not null,
    result tinyint(1) not null,
    checksum varchar(40) not null,
    constraint image_checksum_uindex unique (checksum)
);