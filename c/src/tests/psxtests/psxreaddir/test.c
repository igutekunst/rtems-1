/*
 *  This is a native test to explore how the readdir() family works.
 *  Newlib supports the following readdir() family members:
 *
 *    closedir()   - 
 *    readdir()    - 
 *    scandir()    - 
 *    opendir()    - 
 *    rewinddir()  - 
 *    telldir()    - BSD not in POSIX 
 *    seekdir()    - BSD not in POSIX
 *
 *
 *  seekdir() takes an offset which is a byte offset.  The Linux
 *  implementation of this appears to seek to the ((off/DIRENT_SIZE) + 1)
 *  record where DIRENT_SIZE seems to be 12 bytes.
 *
 *
 *
 *  $Id$
 */
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

#include "/usr1/rtems/mg10/rtems-mg10/c/src/lib/libc/libio_.h"

DIR *directory;
DIR *directory2;
DIR *directory3;
DIR *directory_not;

#ifndef __P
#define __P(args)()
#endif

int scandir ( const char *dirname,
   struct dirent *** namelist,
   int (*select) __P((struct dirent *)),
   int (*dcomp) __P((const void *, const void *))
);

#if defined(__rtems__)
#define d_type d_reclen
#endif

void printdir( DIR *directory )
{
  struct dirent *d;

  printf( "    %-20s %8s %8s %8s %4s\n",
     "     name", "inode", " offset", "reclen", " type" );
  d = readdir(directory);
  
  while (d) {
    printf( "    %-20s %8d %8d %6d   0x%04x\n",
       d->d_name, (int)d->d_ino, (int)d->d_off, d->d_reclen, d->d_type );
    d = readdir(directory);

  }
}

char *many_files[] = {
	"a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i",
        "j",
        "k",
        "l",
        "m",
        "n",
        "o",
        "p",
        "q",
        "r",
        "s",
        "t",
        "u",
        "v",
        "w",
        "x",
        "y",
        "z",
        "aa",
        "ab",
        "ac",
        "ad",
        "ae",
        "af",
        "ag",
        "ah",
        "ai",
        "aj",
        "ak",
        "al",
        "am",
        "an",
        "ao",
        "ap",
        "aq",
        "ar"
};

char *dnames[] = {
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"c/y",
	"c/z",
	"c/x",
	"c/y/a3333",
	"c/y/j123",
        "END"
};

int select1 ( struct dirent *entry )
{
   printf("SCANDIR SELECT1 accepts  nodename: %s\n", entry->d_name );
   return 1;
}

int select2 ( struct dirent *entry )
{
   if( strcmp( entry->d_name, "y") == 0 ) {
      printf("SCANDIR SELECT accepted nodename: %s\n", entry->d_name );
      return 1;
   }
   printf("SCANDIR SELECT rejected nodename: %s\n", entry->d_name );
   return 0;
}

int compare_ascending( struct dirent **a, struct dirent **b )
{
   int i;

   i = strcmp (
      (char *)((struct dirent *)(*a)->d_name),
      (char *)((struct dirent *)(*b)->d_name)
   );
   return i;
}


int compare_descending( struct dirent **a, struct dirent **b )
{
   int i;

   i = strcmp (
      (char *)((struct dirent *)(*b)->d_name),
      (char *)((struct dirent *)(*a)->d_name)
   );

   return i;
}

#if defined(__rtems__)
int test_main(void)
#else
int main(
  int argc,
  char **argv
)
#endif
{
  int fd;
  int i;
  int status;
  off_t off;
  struct dirent *d_not;
  struct dirent **namelist;
  struct stat s;


  printf( "\n\n*** READDIR TEST ***\n" );

  printf( "\nchdir to the root directory\n" );
  status = chdir( "/" );
  printf( "chdir() status : %d\n\n", status );

  printf( "\nCreating a series of directories under /\n" );
  i=0;
  while ( strcmp(dnames[i], "END") != 0 )
  {
     status = mkdir( dnames[i], 0x1c0 );
     printf("Creating directory: %s      %d %d   ", dnames[i], status, errno );
     if ( errno == 0 )
        printf(" Success\n");
     else
        printf(" Failure\n");

     i++;
  }

  status = mkdir( "/many", 0x1c0 );
  status = chdir( "/many" ); 
  for (i = 0; i<=44; i++) {
    fd = open (many_files[i], O_CREAT);
    close (fd);
  }
  directory_not = opendir( "/many" );
  printdir ( directory_not );
  d_not = readdir( directory_not );

  fd = open ("/b/my_file", O_CREAT);
  assert( fd != -1 );
  close (fd);

  status = scandir(
     "/b/my_file",
     &namelist,
     select1,
     NULL
  );

  fd  = open( "/b/new_file", O_CREAT );
  assert( fd != -1 );

  status = fcntl( fd, F_SETFD, 1 );
  assert( status == 0 );
  status = fcntl( fd, F_GETFD, 1 );
  assert( status == 1 );

  status = fcntl( fd, F_DUPFD, 1 );
  assert ( status == -1 );

  status = fcntl( fd, F_GETFL, 1 );
  assert ( status == -1 );

  status = fcntl( fd, F_SETFL, 1 );
  assert ( status == -1 );

  status = fcntl( fd, F_GETLK, 1 );
  assert ( status == -1 );

  status = fcntl( fd, F_SETLK, 1 );
  assert ( status == -1 );

  status = fcntl( fd, F_SETLKW, 1 );
  assert ( status == -1 );

  status = fcntl( fd, F_SETOWN, 1 );
  assert ( status == -1 );

  status = fcntl( fd, F_GETOWN, 1 );
  assert ( status == -1 );

  status = fcntl( fd, 0xb, 1 );
  assert( status == -1 );

  directory_not = opendir ("/b/my_file");
  d_not = readdir(directory_not);

  directory_not = opendir ("/a");
  d_not = readdir (directory_not);

  status = chdir ("/b/my_file");
  assert (status == -1);

  printf( "\nPerforming stat of directory /\n");
  status = stat( "/", &s );
  printf("status for stat : %d, size of directory: %d\n\n",
         status,(int)s.st_size);

  puts( "\nOpening directory /" );
  directory = opendir("/");

  assert( directory );

  printdir(directory);

  printf("\nmkdir /d/my_dir\n");
  status = mkdir( "/d/my_dir", 0x1c0 );
  printf("Open /d/my_dir\n");
  directory_not = opendir( "/d/my_dir" );
  assert( directory_not );

  printf( "remove /d/my_dir.\n" );
  status = rmdir( "/d/my_dir" );
  assert( status == 0 );

  printf( "close /d/my_dir.\n" );
  closedir( directory_not );

  printf( "\nOpening directory /c\n" );
  directory2 = opendir("/c");

  assert( directory2 );

  printdir(directory2);
  status = closedir( directory2 );

  printf( "\nOpening directory /c/y\n" );
  directory3 = opendir("/c/y");

  assert( directory3 );

  printdir(directory3);
  status = closedir( directory3 );

  printf( "\nLSEEK to the start of the open directory\n" );
  lseek( directory->dd_fd, 0, SEEK_SET );
  printdir(directory);

  lseek( directory->dd_fd, 0, SEEK_CUR );

  lseek( directory->dd_fd, 0, SEEK_END );

  lseek( directory->dd_fd, 0, -99 );

  printf( "\nRewinding directory\n" );
  rewinddir( directory );
  printdir(directory);

/* Don't know how to check this one automatically. */
  printf( "Send rewinddir a NULL pointer\n");
  rewinddir( NULL );

  printf( "\nSeek directory\n" );
  printf( "telldir() should report only sizeof(struct dirent) increments \n" );
  printf( "in position. Sizeof(struct dirent): %d\n", sizeof(struct dirent) );
  rewinddir( directory );
  for( off=0 ; off<=200 ; off=off + sizeof(struct dirent) / 4 ) {
    seekdir( directory, off );
    printf( 
       "seeked to %2d -- currently at %2d\n", 
       (int)off, 
       (int)telldir(directory) 
    );
  }

  seekdir( NULL, off );

  printf( "\nClosing directory\n" );
  status = closedir( directory );

  printf( "\nSCANDIR TEST\n");
  printf( "\nselection rule 1\n");
  printf( "scanning for any entry under directory /c\n\n");
  status = scandir(
     "/c",
     &namelist,
     select1,
     NULL
  );
  printf("\nscandir status: %d\n", status );
  for ( i=0; i<status; i++)
  {
     printf("Selected Node Name: %s\n", namelist[i]->d_name );
  }

  printf( "\nselection rule 2\n");
  printf( "scanning for any entry under directory /c whose name = y\n\n");
  status = scandir(
     "/c",
     &namelist,
     select2,
     NULL
  );
  printf("\nscandir status: %d\n", status );
  for ( i=0; i<status; i++)
  {
     printf("Selected Node Name: %s\n", namelist[i]->d_name );
  }

  printf( "\nSCANDIR with sorting\n" );
  printf( "\nselection rule 1\n");
  printf( "scanning for any entry under directory /c\n");
  printf( "sort in ascending order\n\n");
  status = scandir(
     "/c",
     &namelist,
     select1,
     compare_ascending
  );
  printf("\nscandir status: %d\n", status );
  for ( i=0; i<status; i++)
  {
     printf("Selected and Sorted Node Name: %s\n", namelist[i]->d_name );
  }


  printf( "\nSCANDIR with sorting\n" );
  printf( "\nselection rule 1\n");
  printf( "scanning for any entry under directory /c\n");
  printf( "sort in descending order\n\n");
  status = scandir(
     "/c",
     &namelist,
     select1,
     compare_descending
  );
  printf("scandir status: %d\n", status );
  for ( i=0; i<status; i++)
  {
     printf("Selected and Sorted Node Name: %s\n", namelist[i]->d_name );
  }


  printf( "\n\n*** END OF READDIR TEST ***\n" );
  exit(0);
}

