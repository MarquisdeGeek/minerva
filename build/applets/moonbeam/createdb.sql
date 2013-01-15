
drop database if exists moonbeam;
create database moonbeam;

use moonbeam;

CREATE TABLE IF NOT EXISTS links (
  	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	uid INT NOT NULL default 0,
	url VARCHAR(512),
	description VARCHAR(32),
	notes VARCHAR(64)
);

CREATE TABLE IF NOT EXISTS metatags (
        id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	metatag VARCHAR(32)
);

CREATE TABLE IF NOT EXISTS metamap (
	meta_id INT,
	user_id INT,
	link_id INT,
	PRIMARY KEY pk_metamap (meta_id,user_id,link_id)
);


