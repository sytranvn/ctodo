#include "todo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  sqlite3 *db;
  sqlite3_open("todo.db", &db);
  int rc;
  if ((rc = init_db(db)) != SQLITE_OK) {
    exit(rc);
  }
  user_t *user = NULL;
  char *username = getenv("USER");
  if (get_user(db, username, user) == SQLITE_OK && user != NULL) {
    printf("%ld %s\n", user->id, user->name);
  } else {
    user = malloc(sizeof(*user));

    user->name = username;
    user->pass = "Hi";
    if ((rc = add_user(db, user)) != SQLITE_OK) {
      fprintf(stderr, "Add user failed with error code %d\n", rc);
    }
  }
  sqlite3_close(db);
  return 0;
}
