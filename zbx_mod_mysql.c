/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 Copyright (C) 2018 Tomasz Kłoczko <kloczek@fedoraproject.org>
*/

#include <stdarg.h>
#include <time.h>

#include <common.h>
#include <log.h>
#include <module.h>

#include <mysql.h>

/* the variable keeps timeout setting for item processing */
static int item_timeout = 0;

static int zbx_mod_mysql_db_discovery (AGENT_REQUEST * request,
				       AGENT_RESULT * result);
static int zbx_mod_mysql_global_status (AGENT_REQUEST * request,
					AGENT_RESULT * result);
static int zbx_mod_mysql_global_variable (AGENT_REQUEST * request,
					  AGENT_RESULT * result);
static int zbx_mod_mysql_performance_schema (AGENT_REQUEST * request,
					     AGENT_RESULT * result);

static ZBX_METRIC keys[] =
/*	KEY, FLAG, FUNCTION, TEST PARAMETERS */
{
	{"mysql.db.discovery", 0, zbx_mod_mysql_db_discovery, NULL},
	{"mysql.global_status", CF_HAVEPARAMS, zbx_mod_mysql_global_status, ""},
	{"mysql.global_variable", CF_HAVEPARAMS,
	 zbx_mod_mysql_global_variable, ""},
	{"mysql.performance_schema", CF_HAVEPARAMS,
	 zbx_mod_mysql_performance_schema, ""},
	{NULL}
};

/* zbx_mod_mysql is written to interract onl;y with one database */
typedef struct {
	MYSQL *connection;
	char *server;
	char *account;
	char *password;
	unsigned short port;
} ZBX_MOD_MYSQL_INFO;

ZBX_MOD_MYSQL_INFO zbx_mod_mysql_info = {
	NULL,
	"localhost",
	"monitoring",
	"monitoring",
	3306
};

/* Other internal routines */
int zbx_mod_myslq_connect_db ();
void zbx_mod_mysql_close_db ();

/******************************************************************************
 Zabbix items backend routines
******************************************************************************\

/******************************************************************************
 Function:	zbx_mod_mysql_db_discovery

 Purpose:	
 Return value:	SYSINFO_RET_FAIL - function failed, item will be marked
			as not supported by zabbix
		SYSINFO_RET_OK - success
*******************************************************************************/

static int zbx_mod_mysql_db_discovery (AGENT_REQUEST * request,
				       AGENT_RESULT * result)
{
	return SYSINFO_RET_OK;
}

/******************************************************************************
 Function:	zbx_mod_mysql_global_status

 Purpose:	
 Return value:	SYSINFO_RET_FAIL - function failed, item will be marked
			as not supported by zabbix
		SYSINFO_RET_OK - success
*******************************************************************************/

static int zbx_mod_mysql_global_status (AGENT_REQUEST * request,
					AGENT_RESULT * result)
{
	char *param;

	if (1 != request->nparam) {
		/* set optional error message */
		SET_MSG_RESULT (result,
				strdup ("Invalid number of parameters"));
		return SYSINFO_RET_FAIL;
	}

	param = get_rparam (request, 0);

	SET_STR_RESULT (result, strdup (param));

	return SYSINFO_RET_OK;
}

/******************************************************************************
 Function:	zbx_mod_mysql_global_variable

 Purpose:	
 Return value:	SYSINFO_RET_FAIL - function failed, item will be marked
			as not supported by zabbix
		SYSINFO_RET_OK - success
*******************************************************************************/

static int zbx_mod_mysql_global_variable (AGENT_REQUEST * request,
					  AGENT_RESULT * result)
{
	char *param;

	if (1 != request->nparam) {
		/* set optional error message */
		SET_MSG_RESULT (result,
				strdup ("Invalid number of parameters"));
		return SYSINFO_RET_FAIL;
	}

	param = get_rparam (request, 0);

	SET_STR_RESULT (result, strdup (param));

	return SYSINFO_RET_OK;
}

/******************************************************************************
 Function:	zbx_mod_mysql_performance_schema

 Purpose:	
 Return value:	SYSINFO_RET_FAIL - function failed, item will be marked
			as not supported by zabbix
		SYSINFO_RET_OK - success
*******************************************************************************/

static int zbx_mod_mysql_performance_schema (AGENT_REQUEST * request,
					     AGENT_RESULT * result)
{
	char *param;

	if (1 != request->nparam) {
		/* set optional error message */
		SET_MSG_RESULT (result,
				strdup ("Invalid number of parameters"));
		return SYSINFO_RET_FAIL;
	}

	param = get_rparam (request, 0);

	SET_STR_RESULT (result, strdup (param));

	return SYSINFO_RET_OK;
}

/******************************************************************************
 Other internal routines
******************************************************************************\

/******************************************************************************
 Function:	zbx_mod_myslq_connect_db

 Purpose:	Check whether the connection is permanently open and if not try
		to open it. If it fails return with error
 Return value:	SYSINFO_RET_FAIL - function failed, item will be marked
			as not supported by zabbix
		SYSINFO_RET_OK - success
*******************************************************************************/
int zbx_mod_myslq_connect_db ()
{
	if (zbx_mod_mysql_info.connection == NULL) {
		zbx_mod_mysql_info.connection = mysql_init (NULL);
		if (mysql_real_connect (zbx_mod_mysql_info.connection,
					zbx_mod_mysql_info.server,
					zbx_mod_mysql_info.account,
					zbx_mod_mysql_info.password,
					NULL, zbx_mod_mysql_info.port, NULL, 0)
		    ) {
			zabbix_log (LOG_LEVEL_DEBUG,
				    "[zbx_mod_mysql]: zbx_mod_mysql_init_db(): "
				    "A persistent connection established");
			return SYSINFO_RET_OK;
		} else {
			zabbix_log (LOG_LEVEL_ERR,
				    "[zbx_mod_mysql]: mysql_real_connect() error: %s",
				    mysql_error
				    (zbx_mod_mysql_info.connection));
			zbx_mod_mysql_close_db ();
			return SYSINFO_RET_FAIL;
		}
	} else {
		return SYSINFO_RET_OK;
	}
}

/******************************************************************************
 Function:	zbx_mod_mysql_close_db

 Purpose:	Disconnect database and set zbx_mod_mysql_info.connection
		to NULL
 Return value:	Nothing
*******************************************************************************/
void zbx_mod_mysql_close_db ()
{
	if (zbx_mod_mysql_info.connection) {
		/* disconnect database */
		mysql_close (zbx_mod_mysql_info.connection);
		zbx_mod_mysql_info.connection = NULL;
	}
}

/******************************************************************************
 Zabbix loadable module standard ABI routines
******************************************************************************\

/******************************************************************************
 Function:	zbx_module_api_version

 Purpose:	returns version number of the module interface
 Return value:	ZBX_MODULE_API_VERSION - version of module.h module is
                compiled with, in order to load module successfully Zabbix
                MUST be compiled with the same version of this header file
*******************************************************************************/
int zbx_module_api_version (void)
{
	return ZBX_MODULE_API_VERSION;
}

/******************************************************************************
 Function:	zbx_module_item_timeout

 Purpose:	set timeout value for processing of items
 Parameters:	timeout - timeout in seconds, 0 - no timeout set
*******************************************************************************/

void zbx_module_item_timeout (int timeout)
{
	item_timeout = timeout;
}

/******************************************************************************
 Function:	zbx_module_item_list

 Purpose:	returns list of item keys supported by the module
 Return value:	list of item keys
*******************************************************************************/

ZBX_METRIC *zbx_module_item_list (void)
{
	return keys;
}

/******************************************************************************
 Function:	zbx_module_init

 Purpose:	The function is called on agent startup.
		It opens persistent database conectivity and reports to the agent
		logs MySQl client library and server version.
 Return value:	ZBX_MODULE_OK - success
		ZBX_MODULE_FAIL - module initialization failed
 Comment:	the module won't be loaded in case of ZBX_MODULE_FAIL
*******************************************************************************/

int zbx_module_init (void)
{
	if (zbx_mod_myslq_connect_db () == SYSINFO_RET_OK) {
		zabbix_log (LOG_LEVEL_INFORMATION,
			    "[zbx_mod_mysql]: client library version : %s",
			    mysql_get_client_info ());
		zabbix_log (LOG_LEVEL_INFORMATION,
			    "[zbx_mod_mysql]: server version         : %s",
			    mysql_get_server_info
			    (zbx_mod_mysql_info.connection));
	}

	return ZBX_MODULE_OK;
}

/******************************************************************************
 Function:	zbx_module_uninit

 Purpose:	the function is called on agent shutdown
		It should be used to cleanup used resources if there are any
 Return value:	ZBX_MODULE_OK	- success
		ZBX_MODULE_FAIL	- function failed
*******************************************************************************/

int zbx_module_uninit (void)
{
	zbx_mod_mysql_close_db ();

	return ZBX_MODULE_OK;
}
