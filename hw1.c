#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define NUM_CMD 15

int cmd_cat(char **args);
int cmd_cd(char **args);
int cmd_chmod(char **args);
int cmd_echo(char **args);
int cmd_exit(char **args);
int cmd_find(char **args);
int cmd_help(char **args);
int cmd_id(char **args);
int cmd_mkdir(char **args);
int cmd_pwd(char **args);
int cmd_rm(char **args);
int cmd_rmdir(char **args);
int cmd_stat(char **args);
int cmd_touch(char **args);
int cmd_umask(char **args);

char *builtin_str[] = {
  "cat",
  "cd",
  "chmod",
  "echo",
  "exit",
  "find",
  "help",
  "id",
  "mkdir",
  "pwd",
  "rm",
  "rmdir",
  "stat",
  "touch",
  "umask"
};

char *help_str[] = {
    "cat {file}: Display content of {file}. ",
    "cd {dir}: Switch current working directory to {dir}. ",
    "chmod {mode} {file/dir}: Change the mode (permission) of a file or directory. {mode} is an octal number.",
    "echo {str} [filename]: Display {str}. If [filename] is given, open [filename] and append {str} to the file.",
    "exit: Leave the shell.",
    "find [dir]: List files/dirs in the current working directory or [dir] if it is given. Minimum outputs contatin file type, size, and name. ",
    "help: Display help message.",
    "id: Show current euid and egid. ",
    "mkdir {dir}: Create a new directory {dir}. ",
    "pwd: Print the current working directory. ",
    "rm {file}: Remove a file. ",
    "rmdir {dir}: Remove an empty directory. ",
    "stat {file/dir}: Display detailed information of the given file/dir. ",
    "touch {file}: Create {file} if it does not exist, or update its access and modification timestamp. ",
    "umask {mode}: Change the umask of the current session."
};

int (*builtin_func[]) (char **) = {
  &cmd_cat,
  &cmd_cd,
  &cmd_chmod,
  &cmd_echo,
  &cmd_exit,
  &cmd_find,
  &cmd_help,
  &cmd_id,
  &cmd_mkdir,
  &cmd_pwd,
  &cmd_rm,
  &cmd_rmdir,
  &cmd_stat,
  &cmd_touch,
  &cmd_umask
};


/*
    implement function
*/

int cmd_cat(char **args) 
{
  if(args[1] == 0) {
    printf("error: cat {file}\n");
    return 1;
  }
  FILE *f;
  char c;
  f=fopen(args[1],"rt");
  while((c=fgetc(f))!=EOF){
      printf("%c",c);
  }
  printf("\n");
  fclose(f);
  return 1;
} // end of cmd_cat()


int cmd_cd(char **args)
{
  if (args[1] == NULL) {
    printf("error: cd {dir}\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("error: cd");
    }
  }
  return 1;
} // end of cmd_cd()

int cmd_chmod(char **args) 
{
  if(args[1] == 0) {
    printf("error: Please give a octal number and filename, chmod {mode} {filename}\n");
    return 1;
  }
  if(args[2] == 0) {
    printf("error: Please give a octal number and filename, chmod {mode} {filename}\n");
    return 1;
  }
  unsigned int mode = strtoul(args[1],0,8);
  if(chmod(args[2], mode) < 0) {
    printf("error!\n");
    return 1;
  }
  return 1;
} // end of cmd_chmod()

int cmd_echo(char **args) 
{
  if(args[1] == 0) {
    printf("error: echo {str} [filename]\n");
    return 1;
  }
  if(args[1] != 0 && args[2] == 0) {
    printf("%s\n", args[1]);
  }else if(args[1] != 0 && args[2] != 0){
    FILE *f = fopen(args[2], "a");
    fprintf(f, "%s", args[1]);
    fclose(f);
  }
  return 1;
} // end of cmd_echo()

int cmd_exit(char **args)
{
  printf("Bye Bye\n");
  return 0;
} // end of cmd_exit()

int cmd_find(char **args)
{
  DIR *d;
  struct dirent *dir;
  if(args[1] == 0){
    d = opendir(".");
  }else {
    d = opendir(args[1]);
  }
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return 1;
} // end of cmd_find()


int cmd_help(char **args)
{
  int i;

  for (i = 0; i < NUM_CMD; i++) {
    printf("    %s\n", help_str[i]);
  }

  return 1;
} // end of cmd_help()

int cmd_id(char **args)
{
  unsigned int uid = getuid();
  unsigned int gid = getgid();
  printf("uid is %d\n", uid);
  printf("gid is %d\n", gid);
  return 1;
} // end of cmd_id()


int cmd_mkdir(char **args)
{
  if(args[1] == 0) {
    printf("error: mkdir {dir}\n");
    return 1;
  }
  struct stat st = {0};
  if (stat(args[1], &st) == -1) {
      mkdir(args[1], 0777);
  }else {
    printf("error: %s is exist\n", args[1]);
    return 1;
  }
  return 1;
} // end of cmd_mkdir()

int cmd_pwd(char **args)
{
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("%s\n", cwd);
  return 1;
} // end of cmd_pwd()

int cmd_rm(char **args)
{
  if(args[1] == 0) {
    printf("error: rm {file}\n");
    return 1;
  }
  struct stat st = {0};
  if (stat(args[1], &st) != -1) {
      remove(args[1]);
  }else {
    printf("error: %s is not exist\n", args[1]);
    return 1;
  }
  return 1;
} // end of cmd_rm()

int cmd_rmdir(char **args)
{
  if(args[1] == 0) {
    printf("error: rmdir {dir}\n");
    return 1;
  }
  struct stat st = {0};
  if (stat(args[1], &st) != -1) {
      rmdir(args[1]);
  }else {
    printf("error: %s is not exist\n", args[1]);
    return 1;
  }
  return 1;
} // end of cmd_rmdir()

int cmd_stat(char **args)
{

  struct stat fileStat;
  if(args[1] == 0) {
    printf("error: no parameter, stat {file/dir}\n");
    return 1;
  }
  if(stat(args[1],&fileStat) < 0) {
    printf("error: Can't display information of %s\n", args[1]);
    return 1;
  }
  printf("Information for %s\n",args[1]);
  printf("---------------------------\n");
  printf("File Size: \t\t%d bytes\n", (int)fileStat.st_size);
  printf("Number of Links: \t%d\n", (int)fileStat.st_nlink);
  printf("File inode: \t\t%d\n", (int)fileStat.st_ino);

  printf("File Permissions: \t");
  printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
  printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
  printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
  printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");      
  printf("\n\n");
  return 1;
} // end of cmd_stat()

int cmd_touch(char **args)
{
  if(args[1] == 0) {
    printf("error: please give a name, touch {file}\n");
  }
  FILE *fptr;
  fptr = fopen(args[1], "rb+");
  if(fptr == NULL) //if file does not exist, create it
  {
      fptr = fopen(args[1], "wb");
  }
  return 1;
} // end of cmd_touch()

int cmd_umask(char **args)
{
  if(args[1] == 0) {
    printf("error: Please give a octal number, umask {mode}\n");
    return 1;
  }
  printf("Change the umask to %s\n", args[1]);
  unsigned int mode = strtoul(args[1],0,8);
  umask(mode);
  return 1;
} // end of cmd_umask()

int lsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < NUM_CMD; i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

#define LSH_RL_BUFSIZE 1024

char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
    free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    fflush(stdout);
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}


int main(int argc, char **argv)
{

  lsh_loop();

  return EXIT_SUCCESS;
} // end of main()