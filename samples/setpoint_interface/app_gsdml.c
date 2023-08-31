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

#include "sampleapp_common.h"
#include "app_utils.h"
#include "app_gsdml.h"
#include "app_log.h"
#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******************* Supported modules ***************************/

static const app_gsdml_module_t dap_1 = {
   .id = PNET_MOD_DAP_IDENT,
   .name = "DAP 1",
   .submodules = {
      PNET_SUBMOD_DAP_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_1_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_2_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_3_IDENT,
      PNET_SUBMOD_DAP_INTERFACE_1_PORT_4_IDENT,
      0}};

static const app_gsdml_module_t setpoint_x_module = {
   .id = APP_GSDML_MOD_ID_SETPOINT_X,
   .name = "Setpoint X interface module",
   .submodules = {APP_GSDML_SUBMOD_ID_SETPOINT_X, 0}};

static const app_gsdml_module_t setpoint_y_module = {
   .id = APP_GSDML_MOD_ID_SETPOINT_Y,
   .name = "Setpoint y interface module",
   .submodules = {APP_GSDML_SUBMOD_ID_SETPOINT_Y, 0}};

static const app_gsdml_module_t setpoint_z_module = {
   .id = APP_GSDML_MOD_ID_SETPOINT_Z,
   .name = "Setpoint Z interface module",
   .submodules = {APP_GSDML_SUBMOD_ID_SETPOINT_Z, 0}};

/******************* Supported submodules ************************/

static const app_gsdml_submodule_t dap_indentity_1 = {
   .name = "DAP Identity 1",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_interface_1 = {
   .name = "DAP Interface 1",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_1 = {
   .name = "DAP Port 1",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_1_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_2 = {
   .name = "DAP Port 2",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_2_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_3 = {
   .name = "DAP Port 3",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_3_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t dap_port_4 = {
   .name = "DAP Port 4",
   .api = APP_GSDML_API,
   .id = PNET_SUBMOD_DAP_INTERFACE_1_PORT_4_IDENT,
   .data_dir = PNET_DIR_NO_IO,
   .insize = 0,
   .outsize = 0,
   .parameters = {0}};

static const app_gsdml_submodule_t submod_setpoint_x = {
   .id = APP_GSDML_SUBMOD_ID_SETPOINT_X,
   .name = "Setpoint interface for x-axis",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = APP_GSDML_INPUT_DATA_SETPOINT_SIZE,
   .outsize = APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_ECHO_IDX, 0}};

static const app_gsdml_submodule_t submod_setpoint_y = {
   .id = APP_GSDML_SUBMOD_ID_SETPOINT_Y,
   .name = "Setpoint interface for y-axis",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = APP_GSDML_INPUT_DATA_SETPOINT_SIZE,
   .outsize = APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_ECHO_IDX, 0}};

static const app_gsdml_submodule_t submod_setpoint_z = {
   .id = APP_GSDML_SUBMOD_ID_SETPOINT_Z,
   .name = "Setpoint interface for z-axis",
   .api = APP_GSDML_API,
   .data_dir = PNET_DIR_IO,
   .insize = APP_GSDML_INPUT_DATA_SETPOINT_SIZE,
   .outsize = APP_GSDML_OUTPUT_DATA_SETPOINT_SIZE,
   .parameters = {APP_GSDML_PARAMETER_ECHO_IDX, 0}};

/** List of supported modules */
static const app_gsdml_module_t * app_gsdml_modules[] = {
   &dap_1,
   &setpoint_x_module,
   &setpoint_y_module,
   &setpoint_z_module,
};

/** List of supported submodules */
static const app_gsdml_submodule_t * app_gsdml_submodules[] = {
   &dap_indentity_1,
   &dap_interface_1,
   &dap_port_1,
   &dap_port_2,
   &dap_port_3,
   &dap_port_4,

   &submod_setpoint_x,
   &submod_setpoint_y,
   &submod_setpoint_z,
};

/* List of supported parameters.
 * Note that parameters are submodule attributes.
 * This list contain all parameters while each
 * submodule list its supported parameters using
 * their indexes.
 */
static app_gsdml_param_t app_gsdml_parameters[] = {
   {
      .index = APP_GSDML_PARAMETER_1_IDX,
      .name = "Demo 1",
      .length = APP_GSDML_PARAMETER_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_2_IDX,
      .name = "Demo 2",
      .length = APP_GSDML_PARAMETER_LENGTH,
   },
   {
      .index = APP_GSDML_PARAMETER_ECHO_IDX,
      .name = "Echo gain setting",
      .length = APP_GSDML_PARAMETER_LENGTH,
   }};

const app_gsdml_module_t * app_gsdml_get_module_cfg (uint32_t id)
{
   uint32_t i;
   for (i = 0; i < NELEMENTS (app_gsdml_modules); i++)
   {
      if (app_gsdml_modules[i]->id == id)
      {
         return app_gsdml_modules[i];
      }
   }
   return NULL;
}

const app_gsdml_submodule_t * app_gsdml_get_submodule_cfg (uint32_t id)
{
   uint32_t i;
   for (i = 0; i < NELEMENTS (app_gsdml_submodules); i++)
   {
      if (app_gsdml_submodules[i]->id == id)
      {
         return app_gsdml_submodules[i];
      }
   }
   return NULL;
}

const app_gsdml_param_t * app_gsdml_get_parameter_cfg (
   uint32_t submodule_id,
   uint32_t index)
{
   uint16_t i;
   uint16_t j;

   const app_gsdml_submodule_t * submodule_cfg =
      app_gsdml_get_submodule_cfg (submodule_id);

   if (submodule_cfg == NULL)
   {
      /* Unsupported submodule id */
      return NULL;
   }

   /* Search for parameter index in submodule configuration */
   for (i = 0; submodule_cfg->parameters[i] != 0; i++)
   {
      if (submodule_cfg->parameters[i] == index)
      {
         /* Find parameter configuration */
         for (j = 0; j < NELEMENTS (app_gsdml_parameters); j++)
         {
            if (app_gsdml_parameters[j].index == index)
            {
               return &app_gsdml_parameters[j];
            }
         }
      }
   }

   return NULL;
}
