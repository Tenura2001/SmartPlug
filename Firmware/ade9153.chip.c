/*
  ADE9153A Custom Chip Code 

  Author : Pasindu Sandima
*/

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>



#define REG_CONFIG0                                                         0x0020    /* DSP configuration register. */
#define REG_AIRMS                                                           0x0202    /* Phase A filter-based current rms value updated at 4kSPS. */
#define REG_AVRMS                                                           0x0203    /* Phase A filter-based voltage rms value updated at 4kSPS. */
#define REG_AWATT                                                           0x0204    /* Phase A low-pass filtered total active power updated at 4kSPS. */
#define REG_AVA                                                             0x0206    /* Phase A total apparent power updated at 4kSPS. */
#define REG_AFVAR                                                           0x0207    /* Phase A fundamental reactive power updated at 4kSPS. */
///// TODO: Find the register address of the reg version product from the datasheet and fill
#define REG_VERSION_PRODUCT                                                 0x0242    /* This register indicates the version of the product being used. */
/* 16-bit below */
#define REG_RUN                                                             0x0480    /* Write this register to 1 to start the measurements. */
#define REG_AI_PGAGAIN                                                      0x04B9    /* This register configures the PGA gain for Current Channel A. */

#define REG_READ_ONLY 0x07FF
#define READ          0x1
#define Write         0x0 



typedef struct {
  pin_t    cs_pin;
  pin_t    state_pin;
  bool relay_on;
  uint32_t spi;
  uint8_t  spi_buffer[4];
  uint16_t reg_run, reg_ai_pgagain;
  uint32_t reg_config0, reg_awatt, reg_afvar, reg_ava, reg_airms, reg_avrms;
  bool cmd_received;
  bool spi_reading;
  uint16_t cmd_addr;
} chip_state_t;

static void chip_pin_change(void *user_data, pin_t pin, uint32_t value);
static void chip_spi_done(void *user_data, uint8_t *buffer, uint32_t count);

uint32_t reg_awatt_arr [8]  = {0x3FCE1, 0x3FDE1, 0x3F9E1, 0x3F2E1, 0x3FAE1, 0x3FBE1, 0x3F6E1, 0x3F4E1};
uint32_t reg_afvar_arr [8]  = {0x0AB2, 0x0AC2, 0x0A22, 0x0A52, 0x0BB2, 0x0A92, 0x09B2, 0x0AF2};
uint32_t reg_ava_arr   [8]  = {0x3FFFF, 0x3FDFF, 0x3FCFF, 0x3F3FF, 0x3F1FF, 0x3FBFF, 0x3F9FF, 0x3F8FF};
uint32_t reg_airms_arr [8]  = {0x1FAEF4, 0x1F7EF4, 0x1F1EF4, 0x1F2EF4, 0x1FCEF4, 0x1FAEF4, 0x1F8EF4, 0x1FDEF4};
uint32_t reg_avrms_arr [8]  = {0xFEEFA5, 0xFFFFA5, 0xFFCFA5, 0xFBEFA5, 0xFDEFA5, 0xF2EFA5, 0xFD1FA5, 0xFFEFA5};


int counter_awatt = 0;
int counter_afvar = 0;
int counter_ava = 0 ;
int counter_airms = 0;
int counter_avrms = 0;

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));
  
  chip->cs_pin = pin_init("CS", INPUT_PULLUP);
  chip->state_pin = pin_init("STATE", INPUT_PULLUP);
  chip->relay_on = 0;
  chip->reg_run = 0;
  chip->cmd_received = 0;
  chip->cmd_addr = 0;
  chip->reg_awatt = 0x3FFE1;
  chip->reg_afvar = 0x0AB2;
  chip->reg_ava = 0x3FFFF;
  chip->reg_airms = 0x1FEEF4;
  chip->reg_avrms = 0xFFEFA5;

  const pin_watch_config_t watch_config = {
    .edge = BOTH,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };
  pin_watch(chip->cs_pin, &watch_config);

  const pin_watch_config_t watch_config_2 = {
    .edge = BOTH,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };
  pin_watch(chip->state_pin, &watch_config_2);

  const spi_config_t spi_config = {
    .sck = pin_init("SCK", INPUT),
    .miso = pin_init("MISO", INPUT),
    .mosi = pin_init("MOSI", INPUT),
    .done = chip_spi_done,
    .user_data = chip,
  };
  chip->spi = spi_init(&spi_config);
  
  printf("SPI Chip initialized!\n");

}


void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t*)user_data;
  // Handle CS pin logic
  if (pin == chip->cs_pin) {
    if (value == LOW) {
      printf("SPI chip selected\n");
      chip->spi_buffer[0] = 0xFF; // Some dummy data for the first transfer
      chip->spi_buffer[1] = 0xFF; // Some dummy data for the first transfer
      spi_start(chip->spi, chip->spi_buffer, 2); //First it should send the address
      //Which is of 16 bits ( 2 bytes )
    } else {
      printf("SPI chip deselected\n");
      spi_stop(chip->spi);
    }
  } else if (pin == chip->state_pin) {
    if (value == LOW) {
      printf("Relay OFF\n");
      chip->relay_on = 0;
      chip->reg_awatt = 0x0;
      chip->reg_afvar = 0x0;
      chip->reg_ava   = 0x0;
      chip->reg_airms = 0x0;
      chip->reg_avrms = 0x1090000;
      
    } else {
      printf("Relay ON\n");
      chip->relay_on = 1;
      chip->reg_awatt = 0x3FFE1;
      chip->reg_afvar = 0x0AB2;
      chip->reg_ava = 0x3FFFF;
      chip->reg_airms = 0x1FEEF4;
      chip->reg_avrms = 0xFFEFA5;
    }
  }
}

void chip_spi_done(void *user_data, uint8_t *buffer, uint32_t count) {
  chip_state_t *chip = (chip_state_t*)user_data;
  if (!count) {
    // This means that we got here from spi_stop, and no data was received
    return;
  }

  if(chip->spi_reading) { 
      chip->spi_reading = 0;
      printf("Done Reading\n");
      if (pin_read(chip->cs_pin) == LOW) {
          // Continue with the reading the data
          spi_start(chip->spi, chip->spi_buffer, 2);
      }
      return;
  }

  uint16_t received_data_16 = (buffer[0] << 8) | buffer[1] ;
  uint32_t received_data_32 = (buffer[0] << 24) |  (buffer[1] << 16) | (buffer[2] << 8) | buffer[3] ;

  // Command Format addr[11:0],r(1)/w(0),xxx 

  if (chip->cmd_received)
  { 
    printf("Received Data for Command for reg:%x\t data:%x\n", chip->cmd_addr, received_data_32);
    chip->cmd_received = 0;
    switch (chip->cmd_addr)
    {
    case REG_RUN:
        chip->reg_run = received_data_16;   
        printf("Updated the REG_RUN register with %x\n", received_data_16);
        if (pin_read(chip->cs_pin) == LOW) {
            // Continue with the next command
            spi_start(chip->spi, chip->spi_buffer, 2);
        }
        break;

    case REG_AI_PGAGAIN:
        chip->reg_ai_pgagain = received_data_16;   
        printf("Updated the REG_AI_PGAGAIN register with %x\n", received_data_16);
        if (pin_read(chip->cs_pin) == LOW) {
            // Continue with the next command
            spi_start(chip->spi, chip->spi_buffer, 2);
        }
        break;

    case REG_CONFIG0:
        chip->reg_config0 = received_data_32;   
        printf("Updated the REG_CONFIG0 register with %x\n", received_data_32);
        if (pin_read(chip->cs_pin) == LOW) {
            // Continue with the next command
            spi_start(chip->spi, chip->spi_buffer, 2);
        }
        break;
    
    default:
        break;
    }
    
  }
  else
  {
        uint16_t received_addr = (received_data_16 & 0xFFF0) >> 4 ;
        bool read_write = received_data_16 & 0b1000;  // if 1 then read else write
        printf("Received Command to for reg:%x\t r/w:%x\n", received_addr, read_write);
        switch(received_addr) {
            case REG_RUN:
              if(read_write == READ) {
                  buffer[0] = chip->reg_run >> 8;
                  buffer[1] = chip->reg_run;
                  printf("Read the REG_RUN register : %x\n", chip->reg_run);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 2);
                  }
              } else {
                  chip->cmd_addr = REG_RUN;
                  chip->cmd_received = 1;
                  spi_start(chip->spi, chip->spi_buffer, 2);
              }
              break;

            case REG_VERSION_PRODUCT:
              if(read_write == READ) {
                  for (int i = 0 ; i < 4 ; i++) {
                    buffer[i] = 0x0009153A >> (8*(3-i));
                  }
                  printf("Read the REG_VERSION_PRODUCT register : %x\n", 0x0009153A);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 4);
                  }
              } else {
                chip->cmd_received = 1;
                chip->cmd_addr     = REG_READ_ONLY;
                spi_start(chip->spi, chip->spi_buffer, 4); // Get the write data
              }
              break;

            case REG_AI_PGAGAIN:
              if(read_write == READ) {
                  for (int i = 0 ; i < 2 ; i++) {
                    buffer[i] = chip->reg_ai_pgagain >> (8*(1-i));
                  }
                  printf("Read the REG_AI_PGAGAIN register : %x\n", chip->reg_ai_pgagain);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 2);
                  }
              } else {
                chip->cmd_received = 1;
                chip->cmd_addr     = REG_AI_PGAGAIN;
                spi_start(chip->spi, chip->spi_buffer, 2); // Get the write data
              }
              break;

            case REG_CONFIG0:
              if(read_write == READ) {
                  for (int i = 0 ; i < 4 ; i++) {
                    buffer[i] = chip->reg_config0 >> (8*(3-i));
                  }
                  printf("Read the REG_CONFIG0 register : %x\n", chip->reg_config0);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 4);
                  }
              } else {
                chip->cmd_received = 1;
                chip->cmd_addr     = REG_CONFIG0;
                spi_start(chip->spi, chip->spi_buffer, 4); // Read the data and ignore
              }
              break;

            case REG_AWATT:
              if(read_write == READ) {
                  for (int i = 0 ; i < 4 ; i++) {
                    buffer[i] = chip->reg_awatt >> (8*(3-i));
                  }
                  chip->reg_awatt = chip->relay_on ?  reg_awatt_arr[counter_awatt%8] : 0;
                  counter_awatt+=3;
                  printf("Read the REG_AWATT register : %x\n", chip->reg_awatt);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 4);
                  }
              }
              break;

            case REG_AFVAR:
              if(read_write == READ) {
                  for (int i = 0 ; i < 4 ; i++) {
                    buffer[i] = chip->reg_afvar >> (8*(3-i));
                  }
                  chip->reg_afvar = chip->relay_on ?  reg_afvar_arr[counter_afvar%8] : 0;
                  counter_afvar+=5;
                  printf("Read the REG_AFVAR register : %x\n", chip->reg_afvar);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 4);
                  }
              }
              break;

            case REG_AVA:
              if(read_write == READ) {
                  for (int i = 0 ; i < 4 ; i++) {
                    buffer[i] = chip->reg_ava >> (8*(3-i));
                  }
                  chip->reg_ava = chip->relay_on ?  reg_ava_arr[counter_ava%8] : 0;
                  counter_ava+=7;
                  printf("Read the REG_AVA register : %x\n", chip->reg_ava);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 4);
                  }
              }
              break;


            case REG_AIRMS:
              if(read_write == READ) {
                  for (int i = 0 ; i < 4 ; i++) {
                    buffer[i] = chip->reg_airms >> (8*(3-i));
                  }
                  chip->reg_airms = chip->relay_on ?  reg_airms_arr[counter_airms%8] : 0;
                  counter_airms+=5;
                  printf("Read the REG_AIRMS register : %x\n", chip->reg_airms);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 4);
                  }
              }
              break;


            case REG_AVRMS:
              if(read_write == READ) {
                  for (int i = 0 ; i < 4 ; i++) {
                    buffer[i] = chip->reg_avrms >> (8*(3-i));
                  }
                  chip->reg_avrms = reg_avrms_arr[counter_avrms%8];
                  counter_avrms+=7;
                  printf("Read the REG_AVRMS register : %x\n", chip->reg_avrms);
                  if (pin_read(chip->cs_pin) == LOW) {
                      // Continue with the reading the data
                      chip->spi_reading = 1;
                      spi_start(chip->spi, chip->spi_buffer, 4);
                  }
              }
              break;




            


            default:
            break;
        } 
  } 

  
}
