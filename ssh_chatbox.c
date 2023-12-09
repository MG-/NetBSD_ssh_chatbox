#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int nanslp( int ns )
{
  struct timespec time1, time2;
  time1.tv_sec = 0;
  time1.tv_nsec = ns;
  nanosleep(&time1 , &time2);
  return 0;
}

void help()
{
  printf ("0---------------0\n");
  printf ("| c chatbox 1.0 |\n");
  printf ("0---------------0\n");
  printf ("commands:\n");
  printf ("this info        !h\n");
  printf ("set nickname:    !nick <nickname>\n");
  printf ("status:          !stat\n");
  printf ("end connection   !quit\n");
  printf ("\n");
}

int main()
{
  help();
  system ("stty -echo cbreak");
  system ("touch /var/shm/box.txt");
  fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0 ) | O_NONBLOCK );

  int    i,
         c,
         q = 0,
         pos = 0,
         count = 0,
         cmd_has_space=0,
         loop_delay=1000000;
  char * sp_pos,
         cmd[20] = "",
         line[255] = "",
         nick[15] = "anonymous",
         box_line[255],
         prev_box_line[255],
         box_chr  = 0,
         iostat_output[255],
         k_time[10];
  FILE * box_input,
       * box_output,
       * nick_output,
       * cli_input,
       * k_time_read;

  srand( time(0));
  snprintf(nick, 14, "anonymous%i\n", rand());

  box_output = fopen("/var/shm/box.txt","a");
  fprintf(box_output, "%s entering the chat\n", nick);
  fclose(box_output);

  while ( !q) 
  {
    c = fgetc(stdin);
    while  ( c == -1 )
    {
      nanslp(loop_delay);
      box_input = popen( "tail -n1 /var/shm/box.txt | tr -d '\n'", "r" );
      count=0;
      fgets(box_line, 100, box_input);
      pclose(box_input);
      if ( ! strncmp( box_line, prev_box_line, strlen(box_line)) && strlen(box_line))
      {
      }
      else
      {
        printf ("%s\n", box_line );
        strncpy (prev_box_line, box_line, strlen(box_line));
      }
      c = fgetc(stdin);
    }
	
    switch(c)
    {
      case 32 ... 126:
        line[pos] = c;
        line[pos+1] = 0;
        if ( pos <= 254 ) pos++;
        printf ( "%c", c );
        break;

      case 10:
        pos=0;
        printf ("\n");

        // 0---------------------0
        // | !command processing |
        // 0---------------------0
        if ( line[0] == '!' )
        {
          cmd_has_space = 0;
          for (i=0; i<strlen(line); i++) if (line[i] == ' ') cmd_has_space=1;

          count = 0;
          while ( line[count] != ' ' && count<15 )
          {
            cmd[count] = line[count];
            count++;
          }
          cmd[count] = 0;       
          if (cmd_has_space)
          {
            sp_pos=strchr(line, ' ') + 1;
          }

          if ( ! strcmp( cmd, "!quit" )) q = 1;

          if ( ! strcmp( cmd, "!nick" ))
          {
            if ( ! cmd_has_space )
            {
              snprintf(nick, 14, "anonymous%i\n", rand());
            }
            else
            {
              strncpy(nick, sp_pos, 15);
            }
            printf ("nickname set to %s\n", nick);
          }
          line[0]=0;
        }

        if ( ! strcmp( cmd, "!stat") || ! strcmp( cmd, "!status"  ))
        {
          printf ("[ status ]\n" );
          printf ("nickname: %s\n", nick );
          printf ( "CPU idle %% " );
          cli_input = popen( "iostat | tail -n1 | tr -s ' ' | cut -d ' ' -f 11", "r" );
          fgets(iostat_output, 100, cli_input);
          printf ("%s", iostat_output);
          pclose(cli_input);
          strncpy(cmd, "", 1);
        }

        if ( ! strcmp( cmd, "!help") || ! strcmp( cmd, "!h"  )) help();

        k_time_read=popen("/sbin/sysctl kern.cp_time | cut -d ' ' -f 4 | tr -d ',' | tr -d '\n'", "r" );

        fgets(k_time,8, k_time_read);
        pclose(k_time_read);
        box_output = fopen("/var/shm/box.txt","a");
        if (strlen(line)) fprintf(box_output, "%s:%s: %s\n", k_time, nick, line);
        fclose(box_output);

        line[0]=0;
        break;

      case 8:
        line[pos-1] = 0;
        line[pos] = 0;
        if ( pos >0 )
        { 
          pos--;
          printf ( "\b \b");
        }
        break;
    }
  }

  printf ("end\n");
  system ("stty echo");
}
