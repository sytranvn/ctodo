#include "todo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void repl(sqlite3 *db, user_t *user);

int main() {
  sqlite3 *db;
  sqlite3_open("todo.db", &db);
  int rc;
  if ((rc = init_db(db)) != SQLITE_OK) {
    exit(rc);
  }
  user_t *user = NULL;
  char *username = getenv("USER");
  if ((rc = get_user(db, username, &user)) == SQLITE_DONE && user != NULL) {

    printf("Hi, %s\n", user->name);
  } else {
    user = malloc(sizeof(*user));

    user->name = username;
    user->pass = "42";
    if ((rc = add_user(db, user)) != SQLITE_OK) {
      fprintf(stderr, "Add user failed with error code %d\n", rc);
    } else {
      printf("Created user %s, id: %lld, create time: %s\n", user->name,
             user->id, asctime(&user->create_tm));
    }
  }

  task_t *tasks;
  size_t task_cnt;
  const int time_buf_size = 50;
  char time_buf[time_buf_size];
  if ((rc = get_all_tasks(db, user, &tasks, &task_cnt)) == SQLITE_OK) {
    if (tasks != NULL && task_cnt) {
      printf("You have %ld tasks:\n", task_cnt);
      for (int i = 0; i < task_cnt; i++) {
        printf("id: %lld\n"
               "description: %s\n"
               "completed: %s\n",
               tasks[i].id, tasks[i].description,
               tasks[i].completed ? "yes" : "no");

        strftime(time_buf, time_buf_size, "create time: %Y-%m-%d %H:%M:%S",
                 &tasks[i].create_tm);
        puts(time_buf);
        strftime(time_buf, time_buf_size, "update time: %Y-%m-%d %H:%M:%S",
                 &tasks[i].update_tm);
        puts(time_buf);
        puts("");
      }
    } else
      printf("You have no tasks.\n");
  } else
    printf("Error %d\n", rc);

  while (1) {
    repl(db, user);
  }

  sqlite3_close(db);
  free(user);
  if (tasks != NULL) {
    free(tasks);
  }
  return 0;
}

void repl(sqlite3 *db, user_t *user) {
  int rc;
  printf("> ");
  char *cmd_line;
  char *cmd;
  size_t lc = 0;
  getline(&cmd_line, &lc, stdin);
  printf("%s\n", cmd_line);

  sscanf(cmd_line, "%s", cmd);

  printf("%s\n", cmd);
  if (strcmp(cmd, "a") == 0) {
    printf("Add tasks: ");
    char *str;
    size_t len = 0;
    getline(&str, &len, stdin);
    str[strlen(str) - 1] = '\0';
    task_t *todo = malloc(sizeof(*todo));
    todo->description = str;
    if ((rc = add_task(db, user, todo)) == SQLITE_OK) {
      printf("Added task\n");
    } else {
      printf("Error %d\n", rc);
    }
  } else {
    printf("Invalid command, h for help.\n");
  }
}
