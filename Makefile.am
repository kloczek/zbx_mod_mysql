ACLOCAL_AMFLAGS			= -I m4
AM_DISTCHEC_CONFIGURE_FLAGS	= \
	--enable-maintainer-mode

modules_LTLIBRARIES 		= zbx_mod_mysql.la
modulesdir			= $(ZBX_MODULESDIR)

zbx_config_DATA			= LoadModule=zbx_mod_mysql.so
zbx_configdir			= $(ZBX_SYSCONFDIR)/zabbix_agentd

zbx_mod_mysql_la_CFLAGS		= \
	$(MYSQL_CFLAGS) \
	$(ZABBIX_CFLAGS)
zbx_mod_mysql_la_LDFLAGS	= -module -shared -avoid-version -export-dynamic -no-undefined
zbx_mod_mysql_la_LIBADD		= \
	$(MYSQL_LIBS)
zbx_mod_mysql_la_SOURCES	= \
	zbx_mod_mysql.c

EXTRA_DIST			= \
	.indent.pro \
	$(zbx_config_DATA) \
	zbx_mod_mysql.spec \
	zbx_mod_mysql.spec.in
