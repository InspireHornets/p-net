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
static uint8_t actual_x_data[APP_GSDML_OUTPUT_DATA_SETPOINT_X_SIZE] = {0};
static uint8_t setpoint_y_data[APP_GSDML_INPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t actual_y_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t setpoint_z_data[APP_GSDML_INPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t actual_z_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t setpoint_c_data[APP_GSDML_INPUT_DATA_SETPOINT_SIZE] = {0};
static uint8_t actual_c_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};

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

union Sint32 get_torque (const uint8_t * actual_data)
{
   union Sint32 torque;
   torque.bytes[0] = actual_data[15];
   torque.bytes[1] = actual_data[14];
   torque.bytes[2] = actual_data[13];
   torque.bytes[3] = actual_data[12];

   return torque;
}

union Sint32 get_temperature (const uint8_t * actual_data)
{
   union Sint32 temperature;
   temperature.bytes[0] = actual_data[19];
   temperature.bytes[1] = actual_data[18];
   temperature.bytes[2] = actual_data[17];
   temperature.bytes[3] = actual_data[16];

   return temperature;
}

union Sint32 get_state (const uint8_t * actual_data)
{
   union Sint32 state;
   state.bytes[0] = actual_data[23];
   state.bytes[1] = actual_data[22];
   state.bytes[2] = actual_data[21];
   state.bytes[3] = actual_data[20];

   return state;
}

app_actual_x_data_t get_x_trajectory()
{
   app_actual_x_data_t trajectory;

   uint8_t actual_x1_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};
   memcpy (actual_x1_data, actual_x_data, APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE);
   trajectory.x1_position_um = get_position (actual_x1_data).sint32;
   trajectory.x1_speed_um_s = get_speed (actual_x1_data).sint32;
   trajectory.x1_acceleration_um_s2 = get_acceleration (actual_x1_data).sint32;
   trajectory.x1_torque_mNm = get_torque (actual_x1_data).sint32;
   trajectory.x1_temperature_C = get_temperature (actual_x1_data).sint32;
   trajectory.x1_state = get_state (actual_x1_data).sint32;

   uint8_t actual_x2_data[APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE] = {0};
   memcpy (
      actual_x2_data,
      actual_x_data + APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE,
      APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE);
   trajectory.x2_position_um = get_position (actual_x2_data).sint32;
   trajectory.x2_speed_um_s = get_speed (actual_x2_data).sint32;
   trajectory.x2_acceleration_um_s2 = get_acceleration (actual_x2_data).sint32;
   trajectory.x2_torque_mNm = get_torque (actual_x2_data).sint32;
   trajectory.x2_temperature_C = get_temperature (actual_x2_data).sint32;
   trajectory.x2_state = get_state (actual_x2_data).sint32;

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
   p_setpoint_data->state = CC_TO_BE32 (trajectory.state);
}

app_actual_data_t get_y_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_position (actual_y_data).sint32;
   trajectory.speed_um_s = get_speed (actual_y_data).sint32;
   trajectory.acceleration_um_s2 = get_acceleration (actual_y_data).sint32;
   trajectory.torque_mNm = get_torque (actual_y_data).sint32;
   trajectory.temperature_C = get_temperature (actual_y_data).sint32;
   trajectory.state = get_state (actual_y_data).sint32;

   return trajectory;
}

app_actual_data_t get_z_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_position (actual_z_data).sint32;
   trajectory.speed_um_s = get_speed (actual_z_data).sint32;
   trajectory.acceleration_um_s2 = get_acceleration (actual_z_data).sint32;
   trajectory.torque_mNm = get_torque (actual_z_data).sint32;
   trajectory.temperature_C = get_temperature (actual_z_data).sint32;
   trajectory.state = get_state (actual_z_data).sint32;

   return trajectory;
}

app_actual_data_t get_c_trajectory()
{
   app_actual_data_t trajectory;
   trajectory.position_um = get_position (actual_c_data).sint32;
   trajectory.speed_um_s = get_speed (actual_c_data).sint32;
   trajectory.acceleration_um_s2 = get_acceleration (actual_c_data).sint32;
   trajectory.torque_mNm = get_torque (actual_c_data).sint32;
   trajectory.temperature_C = get_temperature (actual_c_data).sint32;
   trajectory.state = get_state (actual_c_data).sint32;

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
   p_setpoint_data->state = CC_TO_BE32 (trajectory.state);
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

void set_c_trajectory_point (app_setpoint_data_t trajectory)
{
   app_setpoint_data_t * p_setpoint_data =
      (app_setpoint_data_t *)&setpoint_c_data;
   p_setpoint_data->position_um = CC_TO_BE32 (trajectory.position_um);
   p_setpoint_data->speed_um_s = CC_TO_BE32 (trajectory.speed_um_s);
   p_setpoint_data->acceleration_um_s2 =
      CC_TO_BE32 (trajectory.acceleration_um_s2);
   p_setpoint_data->state = CC_TO_BE32 (trajectory.state);
}

app_actual4_data_t get_xyzc_trajectory()
{
   app_actual4_data_t trajectory;
   trajectory.x = get_x_trajectory();
   trajectory.y = get_y_trajectory();
   trajectory.z = get_z_trajectory();
   trajectory.c = get_c_trajectory();

   return trajectory;
}

void set_xyzc_trajectory_point (app_setpoint4_data_t trajectory)
{
   set_x_trajectory_point (trajectory.x);
   set_y_trajectory_point (trajectory.y);
   set_z_trajectory_point (trajectory.z);
   set_c_trajectory_point (trajectory.c);
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
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_C)
   {
      *size = APP_GSDML_INPUT_DATA_SETPOINT_SIZE;
      *iops = PNET_IOXS_GOOD;
      return setpoint_c_data;
   }

   /* Automated RT Tester scenario 2 - unsupported (sub)module */
   return NULL;
}

void log_on_change (
   const uint8_t * new_data,
   uint16_t size,
   const uint8_t * old_data,
   char * prepend_log_msg)
{
   if (!are_arrays_equal (
          new_data,
          size,
          old_data,
          APP_GSDML_OUTPUT_DATA_SETPOINT_X_SIZE))
   {
      uint32_t actual_position = combine_bytes_to_uint32 (&old_data[0]);
      uint32_t actual_speed = combine_bytes_to_uint32 (&old_data[4]);
      uint32_t actual_acceleration = combine_bytes_to_uint32 (&old_data[8]);
      uint32_t actual_torque = combine_bytes_to_uint32 (&old_data[12]);
      uint32_t actual_temperature = combine_bytes_to_uint32 (&old_data[16]);

      APP_LOG_DEBUG (
         "%s -- Position: %i\tVelocity: %i\tAcceleration: %i\tTorque: "
         "%i\tTemperature: %i\n",
         prepend_log_msg,
         actual_position,
         actual_speed,
         actual_acceleration,
         actual_torque,
         actual_temperature);
   }
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
      if (size == APP_GSDML_OUTPUT_DATA_SETPOINT_X_SIZE)
      {
         log_on_change (data, size, actual_x_data, "X");
         memcpy (actual_x_data, data, size);

         return 0;
      }
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_Y)
   {
      if (size == APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE)
      {
         log_on_change (data, size, actual_y_data, "Y");
         memcpy (actual_y_data, data, size);

         return 0;
      }
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_Z)
   {
      if (size == APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE)
      {
         log_on_change (data, size, actual_z_data, "Z");
         memcpy (actual_z_data, data, size);

         return 0;
      }
   }
   else if (submodule_id == APP_GSDML_SUBMOD_ID_SETPOINT_C)
   {
      if (size == APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE)
      {
         log_on_change (data, size, actual_c_data, "C");
         memcpy (actual_c_data, data, size);

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
