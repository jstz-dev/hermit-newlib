// SPDX-FileCopyrightText: 2024 TriliTech <contact@trili.tech>
// SPDX-License-Identifier: MIT

char* __env[] = {"USER=root", "PATH=/bin:/sbin:/usr/bin", "GOTRACEBACK=none", "PWD=/", (char*) 0};
char **environ = __env;