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
                    "update_tm DATETIME DEFAULT CURRENT_TIMESTAMP);",
                    NULL, NULL, &msg);
  if (msg != NULL) {
    fprintf(stderr, "%s\n", msg);
    free(msg);
  }

  rc = sqlite3_exec(db,
                    "CREATE TABLE IF NOT EXISTS tasks ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "user_id INTEGER, "
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
      puser->name = malloc(strlen(v) + 1);
      strcpy(puser->name, v);
    } else if (strcmp("create_tm", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      strptime(v, "%Y-%m-%d %H:%M:%d", &(puser)->create_tm);
    } else if (strcmp("update_tm", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      strptime(v, "%Y-%m-%d %H:%M:%d", &(puser)->update_tm);
    }
  }
}

static void parse_task(sqlite3_stmt *stmt, task_t *ptask) {
  int cols = sqlite3_column_count(stmt);
  const char *col;
  const char *v;
  for (int i = 0; i < cols; ++i) {
    col = sqlite3_column_name(stmt, i);
    if (strcmp("id", col) == 0) {
      ptask->id = sqlite3_column_int64(stmt, i);
    } else if (strcmp("description", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      ptask->description = malloc(strlen(v) + 1);
      strcpy(ptask->description, v);
    } else if (strcmp("completed", col) == 0) {
      ptask->completed = sqlite3_column_int(stmt, i);
    } else if (strcmp("create_tm", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      strptime(v, "%Y-%m-%d %H:%M:%d", &(ptask)->create_tm);
    } else if (strcmp("update_tm", col) == 0) {
      v = (char *)sqlite3_column_text(stmt, i);
      strptime(v, "%Y-%m-%d %H:%M:%d", &(ptask)->update_tm);
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

int add_task(sqlite3 *db, const user_t *user, task_t *task) {
  task->user_id = user->id;
  int rc;
  sqlite3_stmt *stmt = NULL;
  char *sql =
      "INSERT INTO tasks (user_id, description) VALUES (?, ?) RETURNING id;";

  if ((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    printf("Preapre error\n");
    return rc;
  }

  if ((rc = sqlite3_bind_int64(stmt, 1, task->user_id)) != SQLITE_OK) {
    printf("Bind user id error\n");
    return rc;
  }
  if ((rc = sqlite3_bind_text(stmt, 2, task->description, -1, NULL)) !=
      SQLITE_OK) {
    printf("Bind description error\n");
    return rc;
  }
  if ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
    parse_task(stmt, task);
  }

  return sqlite3_finalize(stmt);
}

int get_all_tasks(sqlite3 *db, const user_t *user, task_t **tasks,
                  size_t *cnt) {
  char *sql;
  int rc;
  sqlite3_int64 nrow;

  sqlite3_stmt *stmt;
  sql = "SELECT count(*) FROM tasks WHERE completed = 0;";

  if ((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    fprintf(stderr, "Cannot prepare query %s\n", sql);
    *cnt = 0;
    return rc;
  }
  sqlite3_step(stmt);
  nrow = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  *cnt = (size_t)nrow;
  sql = "SELECT * FROM tasks WHERE completed = 0;";
  if ((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    fprintf(stderr, "Cannot prepare query %s\n", sql);
    *cnt = 0;
    return rc;
  }
  *tasks = malloc(*cnt * sizeof(**tasks));
  int i = 0;
  while ((rc = sqlite3_step(stmt) != SQLITE_DONE)) {
    parse_task(stmt, &(*tasks)[i++]);
  }
  sqlite3_finalize(stmt);
  return 0;
};

int delete_task(sqlite3 *db, const user_t *user, todo_id_t id) { return 0; };

int update_task(sqlite3 *db, const user_t *user, todo_id_t id, task_t *task) {
  return 0;
};
