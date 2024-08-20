#include "todo.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int init_db(sqlite3 *db) {
  char *msg = NULL;
  int rc;
  rc = sqlite3_exec(db,
                    "CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT NOT NULL, "
                    "create_tm DATETIME DEFAULT CURRENT_TIMESTAMP, "
                    "update_tm DATETIME DEFAULT CURRENT_TIMESTAMP);"
                    "--------------------------------------"
                    "CREATE TABLE IF NOT EXISTS tasks ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
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

static void parse_user(sqlite3_stmt *stmt, user_t *puser) {
  int cols = sqlite3_column_count(stmt);
  const char *col;
  const char *v;
  for (int i = 0; i < cols; ++i) {
    col = sqlite3_column_name(stmt, i);
    if (strcmp("id", col) == 0) {
      (puser)->id = sqlite3_column_int64(stmt, i);
    } else if (strcmp("name", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      (puser)->name = malloc((strlen(v) + 1) * sizeof(*v));
      strcpy((puser)->name, v);
    } else if (strcmp("create_tm", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      strptime(v, "%Y-%m-%d %H:%M:%d", &(puser)->create_tm);
    } else if (strcmp("update_tm", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      strptime(v, "%Y-%m-%d %H:%M:%d", &(puser)->create_tm);
    }
  }
}

int get_user(sqlite3 *db, const char *name, user_t **puser) {
  int rc;
  sqlite3_stmt *stmt;
  char *q = "SELECT * FROM users WHERE name = ?;";
  if ((rc = sqlite3_prepare_v2(db, q, -1, &stmt, NULL)) != SQLITE_OK) {
    fprintf(stderr, "Cannot prepare query %s\n", q);
    return rc;
  }
  if ((rc = sqlite3_bind_text(stmt, 1, name, -1, NULL)) != SQLITE_OK) {
    fprintf(stderr, "Cannot bind param %s\n", name);
    return rc;
  }
  if ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
    *puser = malloc(sizeof(**puser));
    parse_user(stmt, *puser);
  }

  sqlite3_finalize(stmt);
  return rc = SQLITE_DONE;
}

int add_user(sqlite3 *db, user_t *user) {
  int rc;
  sqlite3_stmt *stmt = NULL;
  char *sql = "INSERT INTO users (name) VALUES (?) RETURNING *;";
  if ((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    return rc;
  }

  if ((rc = sqlite3_bind_text(stmt, 1, user->name, -1, NULL)) != SQLITE_OK) {
    return rc;
  }
  if ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
    parse_user(stmt, user);
  }

  return sqlite3_finalize(stmt);
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
