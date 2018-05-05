### zbx_mod_mysql

This module uses automake, autoconf, libtool and pkgconfig build framework proposed by me in https://support.zabbix.com/browse/ZBX-11767

## Changelog
0.1.0
- initial release which bases on zabbix dummy.c and contains:
  - fully working and tested build framework which passes test of
```
./boostrap.sh
./configure
make
make dist
rpmbuild -ta zbx_mod_mysql-0.1.0.tar.xz --nodeps
```
  - placeholders for module keys:
    - mysql.db.discovery[]
    - mysql.global_status[]
    - mysql.global_variables[]
    - mysql.performance_schema[]
