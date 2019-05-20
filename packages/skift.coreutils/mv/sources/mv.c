/* Copyright © 2018-2019 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include <skift/iostream.h>
#include <skift/filesystem.h>

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        return filesystem_rename(argv[1], argv[2]);
    }

    return -1;
}
