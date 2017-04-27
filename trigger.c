/*
 *
 *   Authors:
 *    Thierry Danis		<ltdanis@cisco.com>
 *
 *   This software is Copyright 1996,1997 by the above mentioned author(s),
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <reubenhwk@gmail.com>.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <syslog.h>

#include <errno.h>

#include "pvd.h"
#include "log.h"
#include "trigger.h"

// trigger_socket : create a pipe on which companion daemons
// can send messages to trigger various operations on the radvd
// daemon
int trigger_pipe(char *trigger_pipe_name)
{
	int fd;

	unlink(trigger_pipe_name);

	if (mkfifo(trigger_pipe_name, S_IREAD | S_IWRITE) == -1) {
		flog(LOG_ERR, "can not create trigger pipe (%s) : %s",
			trigger_pipe_name,
			strerror(errno));
		return(-1);
	}
	if ((fd = open(trigger_pipe_name, O_RDONLY)) == -1) {
		flog(LOG_ERR, "can not open trigger pipe (%s) : %s",
			trigger_pipe_name,
			strerror(errno));
		return(-1);
	}
	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1) {
		flog(LOG_ERR,
			"can not set trigger pipe to non blocking mode (%s) : %s",
			trigger_pipe_name,
			strerror(errno));
		close(fd);
		return(-1);
	}

	return(fd);
}

// FIXME : handle SIGHUP here (writing end has closed) : we need to avoid
// conflict with the legacy way of using SIGHUP to reload the configuration
// file
void process_trigger_msg(int fd)
{
	char PvdId[1024];
	char msg[1024];	// messages should be much shorter than that
	int n;

	if ((n = read(fd, msg, sizeof(msg) - 1)) > 0) {
		msg[n] = '\0';
	}

	// Messages formats are defined in the following document : ...
	// We have chosen to use text based messages to make it simpler to
	// have triggers implementations written in scripting languages
	if (sscanf(msg, "PVD %[^ ] UPDATED\n", PvdId) == 1) {
		if (strlen(PvdId) > PVDIDNAMSIZ - 1) {
			flog(LOG_INFO, "incorrect trigger message (-%s- too large)", PvdId);
		}
		else {
		}
	}
}
