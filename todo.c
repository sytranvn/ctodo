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

int get_user(sqlite3 *db, const char *name, user_t *user) {
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
    int cols = sqlite3_column_count(stmt);
    user = malloc(sizeof(*user));
    const char *col;
    const char *v;
    for (int i = 0; i < cols; ++i) {
      col = sqlite3_column_name(stmt, i);
      if (strcmp("id", col) == 0) {
        user->id = sqlite3_column_int64(stmt, i);
      } else if (strcmp("name", col) == 0) {
        v = sqlite3_column_text16(stmt, i);
        user->name = malloc(strlen(v) + 1);
        strcpy(user->name, v);
      } else if (strcmp("create_tm", col) == 0) {
        user->create_tm = sqlite3_column_int64(stmt, i);
        /* printf("%s\n", v); */
        /* strptime(v, "%Y-%m-%d %H:%M:%S", user->create_tm); */
      } else if (strcmp("update_tm", col) == 0) {
        /* v = sqlite3_column_text16(stmt, i); */
        /* printf("%s\n", v); */
        /* strptime(sqlite3_column_text16(stmt, i), "%Y-%m-%d %H:%M:%S", */
        /*          user->update_tm); */
        gmtime_r(sqlite3_column_int64(stmt, i), user->update_tm);
      }
    }
  };
  sqlite3_finalize(stmt);
  if (user)
    printf("id: %ld, name: %s, create: %s, update: %s\n", (user)->id,
           (user)->name, asctime(&user->create_tm), asctime(&user->update_tm));
  return rc;
}

int add_user(sqlite3 *db, user_t *user) {
  int rc;
  sqlite3_stmt *stmt = NULL;
  char *sql = "INSERT INTO users (name) VALUES (?);";
  if ((rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) != SQLITE_OK) {
    return rc;
  }

  if ((rc = sqlite3_bind_text(stmt, 1, user->name, -1, NULL)) != SQLITE_OK) {
    return rc;
  }

  if ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
    return rc;
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
