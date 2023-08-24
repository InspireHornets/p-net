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
static uint8_t setpoint_z_data[APP_GSDML_INPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t actual_z_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};

union Sint32 get_position (const uint8_t * actual_data)
{
   union Sint32 position;
   position.bytes[0] = actual_data[3];
   position.bytes[1] = actual_data[2];
   position.bytes[2] = actual_data[1];
   position.bytes[3] = actual_data[0];

   return position;
}

union Sint32 get_speed (const uint8_t * actual_data)
{
   union Sint32 speed;
   speed.bytes[0] = actual_data[7];
   speed.bytes[1] = actual_data[6];
   speed.bytes[2] = actual_data[5];
   speed.bytes[3] = actual_data[4];

   return speed;
}

union Sint32 get_acceleration (const uint8_t * actual_data)
{
   union Sint32 acceleration;
   acceleration.bytes[0] = actual_data[11];
   acceleration.bytes[1] = actual_data[10];
   acceleration.bytes[2] = actual_data[9];
   acceleration.bytes[3] = actual_data[8];

   return acceleration;
}

app_actual_data_t get_x_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_position (actual_x_data).sint32;
   trajectory.speed_um_s = get_speed (actual_x_data).sint32;
   trajectory.acceleration_um_s2 = get_acceleration (actual_x_data).sint32;

   return trajectory;
}

void set_x_trajectory_point (app_setpoint_data_t trajectory)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_x_data;
   p_setpoint_data->position_um = CC_TO_BE32 (trajectory.position_um);
   p_setpoint_data->speed_um_s = CC_TO_BE32 (trajectory.speed_um_s);
   p_setpoint_data->acceleration_um_s2 =
      CC_TO_BE32 (trajectory.acceleration_um_s2);
}

app_actual_data_t get_y_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_position (actual_y_data).sint32;
   trajectory.speed_um_s = get_speed (actual_y_data).sint32;
   trajectory.acceleration_um_s2 = get_acceleration (actual_y_data).sint32;

   return trajectory;
}

app_actual_data_t get_z_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_position (actual_z_data).sint32;
   trajectory.speed_um_s = get_speed (actual_z_data).sint32;
   trajectory.acceleration_um_s2 = get_acceleration (actual_z_data).sint32;

   return trajectory;
}

void set_y_trajectory_point (app_setpoint_data_t trajectory)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_y_data;
   p_setpoint_data->position_um = CC_TO_BE32 (trajectory.position_um);
   p_setpoint_data->speed_um_s = CC_TO_BE32 (trajectory.speed_um_s);
   p_setpoint_data->acceleration_um_s2 =
      CC_TO_BE32 (trajectory.acceleration_um_s2);
}

void set_z_trajectory_point (app_setpoint_data_t trajectory)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_z_data;
   p_setpoint_data->position_um = CC_TO_BE32 (trajectory.position_um);
   p_setpoint_data->speed_um_s = CC_TO_BE32 (trajectory.speed_um_s);
   p_setpoint_data->acceleration_um_s2 =
      CC_TO_BE32 (trajectory.acceleration_um_s2);
}

app_actual3_data_t get_xyz_trajectory()
{
   app_actual3_data_t trajectory;
   trajectory.x = get_x_trajectory();
   trajectory.y = get_y_trajectory();
   trajectory.z = get_z_trajectory();

   return trajectory;
}

void set_xyz_trajectory_point (app_setpoint3_data_t trajectory3)
{
   set_x_trajectory_point (trajectory3.x);
   set_y_trajectory_point (trajectory3.y);
   set_z_trajectory_point (trajectory3.z);
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

      return setpoint_x_data;
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_Y)
   {
      *size = APP_GSDML_INPUT_DATA_SETPOINT_SIZE;
      *iops = PNET_IOXS_GOOD;
      return setpoint_y_data;
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_Z)
   {
      *size = APP_GSDML_INPUT_DATA_SETPOINT_SIZE;
      *iops = PNET_IOXS_GOOD;
      return setpoint_z_data;
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
               "X -- Out 1: %i\tOut 2: %i\tOut 3: %i\tOut 4: "
               "%i\tOut "
               "5:% i\n",
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
               "Y -- Out 1: %i\tOut 2: %i\tOut 3: %i\tOut 4: %i\tOut 5:% i\n",
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
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_Z)
   {
      if (size == APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE)
      {
         if (!are_arrays_equal (
                data,
                size,
                actual_z_data,
                APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE))
         {
            uint32_t actual_position =
               combine_bytes_to_uint32 (&actual_z_data[0]);
            uint32_t actual_speed = combine_bytes_to_uint32 (&actual_z_data[4]);
            uint32_t actual_acc = combine_bytes_to_uint32 (&actual_z_data[8]);
            uint32_t loop_in = combine_bytes_to_uint32 (&actual_z_data[12]);
            uint32_t time = combine_bytes_to_uint32 (&actual_z_data[16]);

            APP_LOG_DEBUG (
               "Z -- Out 1: %i\tOut 2: %i\tOut 3: %i\tOut 4: %i\tOut 5:% i\n",
               actual_position,
               actual_speed,
               actual_acc,
               loop_in,
               time);
         }
         memcpy (actual_z_data, data, size);

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
