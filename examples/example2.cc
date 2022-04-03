#include <filesystem>
#include <iostream>

#include <libgen.h>

#include "sentry.h"

int
main(int argc, char **argv)
{
    char* bin = strdup(argv[0]);
    char* bindir = dirname(bin);

    char db_path[256];
    memset(db_path, 0, 256);
    sentry_options_t *options = sentry_options_new();

    sprintf(db_path, "%s/.sentry-native", bindir);
    sentry_options_set_database_path(options, db_path);

    sentry_options_set_dsn(options, "https://14f34bf8727549ce93c589d14948dea0@o681886.ingest.sentry.io/5945196");
    sentry_options_set_environment(options, "dev");
    sentry_options_set_release(options, "0.0.0-rc");
    sentry_options_set_debug(options, true);

    if (true) {
        char handler_path[256];
        memset(handler_path, 0, 256);
        sprintf(handler_path, "%s/crashpad_build/handler/crashpad_handler", bindir);
        fprintf(stderr, "trying to use handler path %s\n", handler_path);
        sentry_options_set_handler_path(options, handler_path);
    } else {
        const char* handler_path = "./out/basic/crashpad_build/handler/crashpad_handler";
        fprintf(stderr, "trying to use handler path %s\n", handler_path);
        sentry_options_set_handler_path(options, handler_path);
    }
    sentry_init(options);

    if (argc > 1) {
      char source[256];
      memset(source, 0, 256);
      sprintf(source, "%s:%s", argv[0], argv[1]);
      fprintf(stderr, "source will be: %s\n", source);
      sentry_set_tag("source", source);
    } else {
      sentry_set_tag("source", argv[0]);
    }

    sentry_value_t exc = sentry_value_new_exception("c++", argv[0]);
    sentry_value_t event = sentry_value_new_object();
    if (argc > 1) {
      sentry_value_set_by_key(event, "message", sentry_value_new_string(argv[1]));
    } else {
      sentry_value_set_by_key(event, "message", sentry_value_new_string("no args"));
    }
    sentry_event_add_exception(event, exc);
    sentry_capture_event(event);

    sentry_close();

    std::cout << "now to screw things up!\n";

    std::abort();

    return 0;
}
