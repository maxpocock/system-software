#include <unistd.h>
#include <syslog.h>
#include "daemon_task.h"
#include <signal.h>
#include <stdlib.h>

void sig_handler(int sigNum)
{
	if (sigNum == SIGINT) {
		syslog(LOG_INFO, "RECEIVED SIGNAL INTERRUPT, INITIATING BACKUP AND TRANSFER");
		lock_directories();
		//check_file_uploads(); checks that all expected files have bee uploaded
		collect_reports();
		backup_dashboard();
		sleep(30);
		unlock_directories();	
	}

}