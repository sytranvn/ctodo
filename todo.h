#include <sqlite3.h>
#include <stdint.h>
#include <time.h>

typedef uint64_t todo_id_t;

typedef struct {
  todo_id_t id;
  char *name;
  char *pass;
  struct tm create_tm;
  struct tm update_tm;
} user_t;

typedef struct {
  todo_id_t id;
  todo_id_t user_id;
  uint8_t completed;
  struct tm create_tm;
  struct tm update_tm;
  char *description;
} task_t;

int init_db(sqlite3 *db);

int get_user(sqlite3 *db, const char* name, user_t** puser);

int add_user(sqlite3 *db, user_t *user);

int delete_user(sqlite3 *db, todo_id_t id);

int add_task(sqlite3 *db, const user_t *user, task_t *task);

int get_all_tasks(sqlite3 *db, const user_t *user, task_t **tasks, size_t *cnt);

int delete_task(sqlite3 *db, const user_t *user, todo_id_t id);

int update_task(sqlite3 *db, const user_t *user, todo_id_t id, task_t *task);
