/* book: A key:value match, used for bookmarking directories
* Copyright (C) <2016>  <Tyler Stafos>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void print_usage ()
{
    printf ("Requires one argument (key), returns key:$VALUE\n");
    printf ("This is an eager search, meaning, it will return the first\n");
    printf ("value that matches $KEY. For example, if a key for ~/Documents\n");
    printf ("is $KEY='documents', and a key for ~/Downloads is $KEY='downloads'\n");
    printf ("the argument 'do' will return whichever key it finds first.\n");
    printf ("Which would be the first one you marked\n");
}

char* match_key_get_new_value (char* key, char* search_buffer)
{
	if (!(key && search_buffer))
		return NULL;

	char field_del[2];
	field_del[0] = '\n';
	field_del[1] = '\0';

	char *token = NULL;
	char tok_del = ':';
	size_t key_len = strlen (key);
	size_t tky_len = 0;
	size_t tok_len = 0;

	token = strtok (search_buffer, field_del);
	while (token)
	{
		tok_len = strlen (token);
		tky_len = strcspn (token, &tok_del);
		if (tky_len >= key_len)
		{
			if ((strncmp (token, key, key_len) == 0))
			{
				return &token[tky_len+1];		
			}
		}
		
		token = strtok (NULL, field_del);
	}
	return NULL;
}

char* New_get_bookmark_file (char *file_path)
{
	if (!file_path)
		return NULL;
    errno = 0;
	FILE *fp = fopen (file_path, "r");	
	if (!fp)
    {
        perror ("Error: Failed to open file\n");
		return NULL;
    }
	
	int rv = fseek (fp, 0l, SEEK_END);
	if (rv)
	{ 
        fclose (fp); 
        fprintf (stderr, "Error: Failed to access the file\n");
        return NULL; 
    }
	
	long file_size = ftell (fp);
	rewind (fp);
	char* rt_val = malloc (file_size + 1);
	
	size_t tot_len = fread (rt_val, 1, file_size, fp);
	rt_val[tot_len] = '\0';
	fclose (fp);

	return rt_val;	
}

int main (int argc, char **argv)
{
	if (argc == 2)
	{
		char *key       = argv[1];
        // The file name
        char *bk_file   = "/.bookmarks";
		char *file_path = getenv ("HOME");
        if (!(file_path))
        {
            fprintf (stderr, "Error: Could not get $HOME variable\n");
            return 1;
        }

		int  fp_len     = strlen (file_path);
		char *full_path = malloc (fp_len + strlen (bk_file) + 1);
		full_path[0]    = '\0';
		strcat (full_path, file_path);
		strcat (full_path, bk_file);

		char *file_buf  = New_get_bookmark_file (full_path);
        if (!(file_buf))
        {
            fprintf (stderr, "Error: Failure in reading file\n");
            return 1;
        }

		char *val       = match_key_get_new_value (key, file_buf);

		if (val)
			printf ("%s\n", val);
		else
			printf ("did not match\n");
		return 0;
	}
    print_usage ();
	return 1;
}
