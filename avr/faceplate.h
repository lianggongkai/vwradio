#ifndef FACEPLATE_H
#define FACEPLATE_H

#include <stdint.h>
#include "updemu.h"

void faceplate_spi_init();
void faceplate_clear_display();
void faceplate_send_upd_command();
void faceplate_write_upd_ram(uint8_t data_setting_cmd,
    uint8_t data_size, uint8_t *data);
void faceplate_update_from_upd_if_dirty(upd_state_t *state);

#endif