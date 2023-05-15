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
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "sampleapp_common.h"
#include "osal.h"
#include "pnal.h"
#include "utils.h"
#include <pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
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
static uint8_t setpoint_data[APP_GSDML_INPUT_DATA_ECHO_SIZE] = {0};
static uint8_t actual_data[APP_GSDML_OUTPUT_DATA_ECHO_SIZE] = {0};

CC_PACKED_BEGIN
typedef struct CC_PACKED app_setpoint_data
{
   /* Network endianness */
   uint32_t position_um;
   uint32_t speed_mm_min;
   uint32_t acceleration_mm_min2;
   uint32_t state;
} app_setpoint_data_t;
CC_PACKED_END
CC_STATIC_ASSERT (
   sizeof (app_setpoint_data_t) == APP_GSDML_INPUT_DATA_ECHO_SIZE);

CC_PACKED_BEGIN
typedef struct CC_PACKED app_actual_data
{
   /* Network endianness */
   uint32_t position_um;
   uint32_t speed_mm_min;
   uint32_t acceleration_mm_min2;
   uint32_t power;
   uint32_t temperature;
} app_actual_data_t;
CC_PACKED_END
CC_STATIC_ASSERT (sizeof (app_actual_data_t) == APP_GSDML_OUTPUT_DATA_ECHO_SIZE);

// ToDO add get_x, get_y
uint32_t get_x()
{
   return combine_bytes_to_uint32 (&actual_data[0]);
}

union Unint32 get_x_position()
{
   union Unint32 x_pos;
   x_pos.bytes[0] = actual_data[3];
   x_pos.bytes[1] = actual_data[2];
   x_pos.bytes[2] = actual_data[1];
   x_pos.bytes[3] = actual_data[0];

   return x_pos;
}

union Unint32 get_x_speed()
{
   union Unint32 x_speed;
   x_speed.bytes[0] = actual_data[7];
   x_speed.bytes[1] = actual_data[6];
   x_speed.bytes[2] = actual_data[5];
   x_speed.bytes[3] = actual_data[4];

   return x_speed;
}

union Unint32 get_x_acceleration()
{
   union Unint32 x_acceleration;
   x_acceleration.bytes[0] = actual_data[7];
   x_acceleration.bytes[1] = actual_data[6];
   x_acceleration.bytes[2] = actual_data[5];
   x_acceleration.bytes[3] = actual_data[4];

   return x_acceleration;
}

void set_x (uint32_t setpoint)
{
   app_actual_data_t * p_actual_data = (app_actual_data_t *)&setpoint_data;
   p_actual_data->position_um = CC_TO_BE32 (setpoint);
}

// void set_y (uint32_t setpoint)
//{
//    app_echo_data_t * p_echo_inputdata = (app_echo_data_t *)&setpoint_data;
//    p_echo_inputdata->speed_mm_min = CC_TO_BE32 (setpoint);
// }

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

   if (submodule_id == APP_GSDML_SUBMOD_ID_ECHO)
   {
      *size = APP_GSDML_INPUT_DATA_ECHO_SIZE;
      *iops = PNET_IOXS_GOOD;
      return setpoint_data;
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

   if (submodule_id == APP_GSDML_SUBMOD_ID_ECHO)
   {
      if (size == APP_GSDML_OUTPUT_DATA_ECHO_SIZE)
      {
         if (!are_arrays_equal (
                data,
                size,
                actual_data,
                APP_GSDML_OUTPUT_DATA_ECHO_SIZE))
         {
            for (int i = 0; i < APP_GSDML_OUTPUT_DATA_ECHO_SIZE; i++)
            {
               uint32_t actual_position =
                  combine_bytes_to_uint32 (&actual_data[0]);
               uint32_t actual_speed =
                  combine_bytes_to_uint32 (&actual_data[4]);

               APP_LOG_DEBUG (
                  "New actual position 1: %u\tactual position 2: %u\n",
                  actual_position,
                  actual_speed);
            }
         }
         memcpy (actual_data, data, size);

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
