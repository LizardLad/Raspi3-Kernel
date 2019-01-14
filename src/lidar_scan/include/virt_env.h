#ifndef VIRT_ENV_H
#define VIRT_ENV_H

#include <stdint.h>
#include <stdbool.h>

extern int32_t SCX, SCY, SCZ, CX, CY, CZ; //World Dimentions
extern char *virt_env;
extern bool env_loaded; 

int init_virt_env();
inline void env_set(int32_t x, int32_t y, int32_t z, uint8_t val);
inline uint8_t env_get(int32_t x, int32_t y, int32_t z);

inline void out_env_set(int32_t x, int32_t y, int32_t z, uint8_t val);

inline void set_env_loaded(bool val);
inline bool get_env_loaded();
inline bool is_env_loaded();

#endif