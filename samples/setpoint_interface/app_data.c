/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2021 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include "app_data.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "utils.h"

#include <string.h>

#define APP_DATA_DEFAULT_OUTPUT_DATA 0

/* Parameter data for digital submodules
 * The stored value is shared between all digital submodules in this example.
 *
 * Todo: Data is always in pnio data format. Add conversion to uint32_t.
 */
static uint32_t app_param_1 = 0; /* Network endianness */
static uint32_t app_param_2 = 0; /* Network endianness */

/* Parameter data for echo submodules
 * The stored value is shared between all echo submodules in this example.
 *
 * Todo: Data is always in pnio data format. Add conversion to uint32_t.
 */
static uint32_t app_param_echo_gain = 1; /* Network endianness */

/* Network endianness */
static uint8_t setpoint_x_data[APP_GSDML_INPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t actual_x_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t setpoint_y_data[APP_GSDML_INPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t actual_y_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};

static int32_t counter = 0;

union Sint32 get_x_position()
{
   union Sint32 x_pos;
   x_pos.bytes[0] = actual_x_data[3];
   x_pos.bytes[1] = actual_x_data[2];
   x_pos.bytes[2] = actual_x_data[1];
   x_pos.bytes[3] = actual_x_data[0];

   return x_pos;
}

union Sint32 get_x_speed()
{
   union Sint32 x_speed;
   x_speed.bytes[0] = actual_x_data[7];
   x_speed.bytes[1] = actual_x_data[6];
   x_speed.bytes[2] = actual_x_data[5];
   x_speed.bytes[3] = actual_x_data[4];

   return x_speed;
}

union Sint32 get_x_acceleration()
{
   union Sint32 x_acceleration;
   x_acceleration.bytes[0] = actual_x_data[11];
   x_acceleration.bytes[1] = actual_x_data[10];
   x_acceleration.bytes[2] = actual_x_data[9];
   x_acceleration.bytes[3] = actual_x_data[8];

   return x_acceleration;
}

union Sint32 get_x_power()
{
   union Sint32 x_power;
   x_power.bytes[0] = actual_x_data[15];
   x_power.bytes[1] = actual_x_data[14];
   x_power.bytes[2] = actual_x_data[13];
   x_power.bytes[3] = actual_x_data[12];

   return x_power;
}

union Sint32 get_x_temperature()
{
   union Sint32 x_temperature;
   x_temperature.bytes[0] = actual_x_data[19];
   x_temperature.bytes[1] = actual_x_data[18];
   x_temperature.bytes[2] = actual_x_data[17];
   x_temperature.bytes[3] = actual_x_data[16];

   return x_temperature;
}

app_actual_data_t get_x_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_x_position().sint32;
   trajectory.speed_mm_min = get_x_speed().sint32;
   trajectory.acceleration_mm_min2 = get_x_acceleration().sint32;

   return trajectory;
}

void set_x_position (int32_t setpoint)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_x_data;
   p_setpoint_data->position_um = CC_TO_BE32 (setpoint);
}

void set_x_state (int32_t state)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_x_data;
   p_setpoint_data->state = CC_TO_BE32 (state);
}

void set_x_trajectory_point (app_setpoint_data_t trajectory)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_x_data;
   p_setpoint_data->position_um = CC_TO_BE32 (trajectory.position_um);
   p_setpoint_data->speed_mm_min = CC_TO_BE32 (trajectory.speed_mm_min);
   p_setpoint_data->acceleration_mm_min2 =
      CC_TO_BE32 (trajectory.acceleration_mm_min2);
}

union Sint32 get_y_position()
{
   union Sint32 y_pos;
   y_pos.bytes[0] = actual_y_data[3];
   y_pos.bytes[1] = actual_y_data[2];
   y_pos.bytes[2] = actual_y_data[1];
   y_pos.bytes[3] = actual_y_data[0];

   return y_pos;
}

union Sint32 get_y_speed()
{
   union Sint32 y_speed;
   y_speed.bytes[0] = actual_y_data[7];
   y_speed.bytes[1] = actual_y_data[6];
   y_speed.bytes[2] = actual_y_data[5];
   y_speed.bytes[3] = actual_y_data[4];

   return y_speed;
}

union Sint32 get_y_acceleration()
{
   union Sint32 y_acceleration;
   y_acceleration.bytes[0] = actual_y_data[11];
   y_acceleration.bytes[1] = actual_y_data[10];
   y_acceleration.bytes[2] = actual_y_data[9];
   y_acceleration.bytes[3] = actual_y_data[8];

   return y_acceleration;
}

app_actual_data_t get_y_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_y_position().sint32;
   trajectory.speed_mm_min = get_y_speed().sint32;
   trajectory.acceleration_mm_min2 = get_y_acceleration().sint32;

   return trajectory;
}

void set_y_trajectory_point (app_setpoint_data_t trajectory)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_y_data;
   p_setpoint_data->position_um = CC_TO_BE32 (trajectory.position_um);
   p_setpoint_data->speed_mm_min = CC_TO_BE32 (trajectory.speed_mm_min);
   p_setpoint_data->acceleration_mm_min2 =
      CC_TO_BE32 (trajectory.acceleration_mm_min2);
}

uint8_t * app_data_to_plc (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   uint16_t * size,
   uint8_t * iops)
{
   if (size == NULL || iops == NULL)
   {
      return NULL;
   }

   if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_X)
   {
      *size = APP_GSDML_INPUT_DATA_SETPOINT_SIZE;
      *iops = PNET_IOXS_GOOD;

      //      counter += 1;
      //      uint32_t counter_network_endianess = CC_TO_BE32 (counter);
      //      memcpy (&setpoint_x_data[12], &counter_network_endianess, 4);

      return setpoint_x_data;
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_Y)
   {
      *size = APP_GSDML_INPUT_DATA_SETPOINT_SIZE;
      *iops = PNET_IOXS_GOOD;
      return setpoint_y_data;
   }

   /* Automated RT Tester scenario 2 - unsupported (sub)module */
   return NULL;
}

int app_data_from_plc (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   uint8_t * data,
   uint16_t size)
{
   if (data == NULL)
   {
      return -1;
   }

   if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_X)
   {
      if (size == APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE)
      {
         if (!are_arrays_equal (
                data,
                size,
                actual_x_data,
                APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE))
         {
            uint32_t actual_position =
               combine_bytes_to_uint32 (&actual_x_data[0]);
            uint32_t actual_speed = combine_bytes_to_uint32 (&actual_x_data[4]);
            uint32_t actual_acc = combine_bytes_to_uint32 (&actual_x_data[8]);
            uint32_t loop_in = combine_bytes_to_uint32 (&actual_x_data[12]);
            uint32_t time = combine_bytes_to_uint32 (&actual_x_data[16]);

            APP_LOG_DEBUG (
               "X -- Counter: %i, Out 1: %i\tOut 2: %i\tOut 3: %i\tOut 4: "
               "%i\tOut "
               "5:% i\n ",
               counter,
               actual_position,
               actual_speed,
               actual_acc,
               loop_in,
               time);
         }
         memcpy (actual_x_data, data, size);

         return 0;
      }
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_Y)
   {
      if (size == APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE)
      {
         if (!are_arrays_equal (
                data,
                size,
                actual_y_data,
                APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE))
         {
            uint32_t actual_position =
               combine_bytes_to_uint32 (&actual_y_data[0]);
            uint32_t actual_speed = combine_bytes_to_uint32 (&actual_y_data[4]);
            uint32_t actual_acc = combine_bytes_to_uint32 (&actual_y_data[8]);
            uint32_t loop_in = combine_bytes_to_uint32 (&actual_y_data[12]);
            uint32_t time = combine_bytes_to_uint32 (&actual_y_data[16]);

            APP_LOG_DEBUG (
               "Y -- Out 1: %i\tOut 2: %i\tOut 3: %i\tOut 4: %i\tOut 5:% i\n ",
               actual_position,
               actual_speed,
               actual_acc,
               loop_in,
               time);
         }
         memcpy (actual_y_data, data, size);

         return 0;
      }
   }

   return -1;
}

int app_data_write_parameter (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   uint32_t index,
   const uint8_t * data,
   uint16_t length)
{
   const app_gsdml_param_t * par_cfg;

   par_cfg = app_gsdml_get_parameter_cfg (submodule_id, index);
   if (par_cfg == NULL)
   {
      APP_LOG_WARNING (
         "PLC write request unsupported submodule/parameter. "
         "Submodule id: %u Index: %u\n",
         (unsigned)submodule_id,
         (unsigned)index);
      return -1;
   }

   if (length != par_cfg->length)
   {
      APP_LOG_WARNING (
         "PLC write request unsupported length. "
         "Index: %u Length: %u Expected length: %u\n",
         (unsigned)index,
         (unsigned)length,
         par_cfg->length);
      return -1;
   }

   if (index == APP_GSDML_PARAMETER_1_IDX)
   {
      memcpy (&app_param_1, data, length);
   }
   else if (index == APP_GSDML_PARAMETER_2_IDX)
   {
      memcpy (&app_param_2, data, length);
   }
   else if (index == APP_GSDML_PARAMETER_ECHO_IDX)
   {
      memcpy (&app_param_echo_gain, data, length);
   }

   APP_LOG_DEBUG ("  Writing parameter \"%s\"\n", par_cfg->name);
   app_log_print_bytes (APP_LOG_LEVEL_DEBUG, data, length);

   return 0;
}

int app_data_read_parameter (
   uint16_t slot_nbr,
   uint16_t subslot_nbr,
   uint32_t submodule_id,
   uint32_t index,
   uint8_t ** data,
   uint16_t * length)
{
   const app_gsdml_param_t * par_cfg;

   par_cfg = app_gsdml_get_parameter_cfg (submodule_id, index);
   if (par_cfg == NULL)
   {
      APP_LOG_WARNING (
         "PLC read request unsupported submodule/parameter. "
         "Submodule id: %u Index: %u\n",
         (unsigned)submodule_id,
         (unsigned)index);
      return -1;
   }

   if (*length < par_cfg->length)
   {
      APP_LOG_WARNING (
         "PLC read request unsupported length. "
         "Index: %u Length: %u Expected length: %u\n",
         (unsigned)index,
         (unsigned)*length,
         par_cfg->length);
      return -1;
   }

   APP_LOG_DEBUG ("  Reading \"%s\"\n", par_cfg->name);
   if (index == APP_GSDML_PARAMETER_1_IDX)
   {
      *data = (uint8_t *)&app_param_1;
      *length = sizeof (app_param_1);
   }
   else if (index == APP_GSDML_PARAMETER_2_IDX)
   {
      *data = (uint8_t *)&app_param_2;
      *length = sizeof (app_param_2);
   }
   else if (index == APP_GSDML_PARAMETER_ECHO_IDX)
   {
      *data = (uint8_t *)&app_param_echo_gain;
      *length = sizeof (app_param_echo_gain);
   }

   app_log_print_bytes (APP_LOG_LEVEL_DEBUG, *data, *length);

   return 0;
}
