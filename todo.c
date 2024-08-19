#include "todo.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

int init_db(sqlite3 *db) {
  char *msg = NULL;
  int rc;
  rc = sqlite3_exec(db,
                    "CREATE TABLE IF NOT EXISTS users ("
                    "id UNSIGNED BIG INT PRIMARY KEY, "
                    "name TEXT NOT NULL, "
                    "create_tm DATETIME DEFAULT CURRENT_TIMESTAMP, "
                    "update_tm DATETIME DEFAULT CURRENT_TIMESTAMP);"
                    "--------------------------------------"
                    "CREATE TABLE IF NOT EXISTS tasks ("
                    "id UNSIGNED BIG INT PRIMARY KEY, "
                    "description TEXT NOT NULL, "
                    "completed SMALLINT DEFAULT 0,"
                    "create_tm DATETIME DEFAULT CURRENT_TIMESTAMP, "
                    "update_tm DATETIME DEFAULT CURRENT_TIMESTAMP);",
                    NULL, NULL, &msg);
  if (msg != NULL) {
    fprintf(stderr, "%s\n", msg);
    free(msg);
  }
  return rc;
}

int add_user(sqlite3 *db, user_t *user) {
  int rc;
  sqlite3_stmt *stmt;
  rc = sqlite3_prepare(
      db, "INSERT INTO users (name, create_tm, update_tm) VALUES (%s, %s, %s);",
      -1, &stmt, NULL);
  return rc;
};

int delete_user(sqlite3 *db, todo_id_t id) { return 0; };

int add_task(sqlite3 *db, const user_t *user, task_t *task) { return 0; };

int get_all_tasks(sqlite3 *db, const user_t *user, task_t **tasks,
                  size_t *cnt) {
  task_t *task;
  *tasks = malloc(sizeof(tasks));
  task = *tasks;
  task->user_id = 1;
  task->id = 1;
  task->description = "Hello";
  *cnt = 1;
  return 0;
};

int delete_task(sqlite3 *db, const user_t *user, todo_id_t id) { return 0; };

int update_task(sqlite3 *db, const user_t *user, todo_id_t id, task_t *task) {
  return 0;
};
