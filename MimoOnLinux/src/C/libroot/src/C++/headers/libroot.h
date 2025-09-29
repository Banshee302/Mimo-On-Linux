#ifndef LIBROOT_H
#define LIBROOT_H

#ifdef __cplusplus
extern "C" {
#endif

void init_libroot();
int verify_libroot_password();
int run_as_root(const char* appName);
int librootmain(int argc, char* argv[]);  // <-- Add this line

#ifdef __cplusplus
}
#endif

#endif // LIBROOT_H

