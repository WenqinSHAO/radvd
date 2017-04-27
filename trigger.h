/*
 *
 *   Authors:
 *    Thierry Danis		<tdanis@cisco.com>
 *
 *   This software is Copyright 1996,1997 by the above mentioned author(s),
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <reubenhwk@gmail.com>.
 *
 */

#pragma once

void process_trigger_msg(int fd);
int trigger_pipe(char *trigger_pipe_name);
