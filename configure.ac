#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([zbx_mod_mysql],
	[0.0.0],
	[https://github.com/kloczek/zbx_mod_mysql/issues],
	[],
	[https://github.com/kloczek/zbx_mod_mysql])
#AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([dist-xz no-dist-gzip no-dependencies])

AM_MAINTAINER_MODE([disable])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_DISABLE_STATIC
AM_PROG_LIBTOOL

PKG_CHECK_MODULES([ZABBIX], [zabbix-1.0],
	[PKG_CHECK_VAR([ZBX_MODULESDIR], [zabbix-1.0], [modulesdir])
	PKG_CHECK_VAR([ZBX_SYSCONFDIR], [zabbix-1.0], [sysconfdir])
	]
)

# Checks for libraries.
PKG_CHECK_MODULES([MYSQL], [mysqlclient])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT([
	Makefile
	zbx_mod_mysql.spec
])
