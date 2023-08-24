/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2018 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#ifndef SAMPLEAPP_COMMON_H
#define SAMPLEAPP_COMMON_H

#include "osal.h"
#include "pnal.h"
#include <pnet_api.h>

#ifdef __cplusplus
extern "C" {
#endif

#define APP_TICK_INTERVAL_US 1000 /* 1 ms */

/* Thread configuration for targets where sample
 * event loop is run in a separate thread (not main).
 * This applies for linux sample app implementation.
 */
#define APP_MAIN_THREAD_PRIORITY  15
#define APP_MAIN_THREAD_STACKSIZE 4096 /* bytes */

#define APP_DATA_LED_ID            1
#define APP_PROFINET_SIGNAL_LED_ID 2

#define APP_TICKS_READ_BUTTONS 10
// half the PLC cycle time to give the UDP time to update
#define APP_TICKS_UPDATE_DATA 8

#define APP_UDP_MESSAGE_LENGTH 64
#define APP_UDP_HOST_ADDRESS   "127.0.0.1"
#define APP_UDP_PORT           2000

/** HW Offload configuration. */
typedef enum
{
   MODE_HW_OFFLOAD_NONE = 0,
   MODE_HW_OFFLOAD_CPU,
   MODE_HW_OFFLOAD_FULL,
} app_mode_t;

/** Command line arguments for sample application */
typedef struct app_args
{
   char path_storage_directory[PNET_MAX_DIRECTORYPATH_SIZE]; /** Terminated */
   char station_name[PNET_STATION_NAME_MAX_SIZE]; /** Terminated string */
   char eth_interfaces
      [PNET_INTERFACE_NAME_MAX_SIZE * (PNET_MAX_PHYSICAL_PORTS + 1) +
       PNET_MAX_PHYSICAL_PORTS]; /** Terminated string */
   int verbosity;
   int show;
   bool factory_reset;
   bool remove_files;
   app_mode_t mode;
} app_args_t;

typedef enum
{
   RUN_IN_SEPARATE_THREAD,
   RUN_IN_MAIN_THREAD
} app_run_in_separate_task_t;

typedef struct app_data_t app_data_t;

/** Partially initialise config values, and use proper callbacks
 *
 * @param pnet_cfg     Out:   Configuration to be updated
 */
void app_pnet_cfg_init_default (pnet_cfg_t * pnet_cfg);

/**
 * Initialize P-Net stack and application.
 *
 * The \a pnet_cfg argument shall have been initialized using
 * \a app_pnet_cfg_init_default() before this function is
 * called.
 *
 * @param pnet_cfg               In:    P-Net configuration
 * @param app_args               In:    Application arguments
 * @return Application handle, NULL on error
 */
app_data_t * app_init (const pnet_cfg_t * pnet_cfg, const app_args_t * app_args);

/**
 * Start application main loop
 *
 * Application must have been initialized using \a app_init() before
 * this function is called.
 *
 * If \a task_config parameters is set to RUN_IN_SEPARATE_THREAD a
 * thread execution the \a app_loop_forever() function is started.
 * If task_config is set to RUN_IN_MAIN_THREAD no such thread is
 * started and the caller must call the \a app_loop_forever() after
 * calling this function.
 *
 * RUN_IN_MAIN_THREAD is intended for rt-kernel targets.
 * RUN_IN_SEPARATE_THREAD is intended for linux targets.
 *
 * @param app                 In:    Application handle
 * @param task_config         In:    Defines if stack and application
 *                                   is run in main or separate task.
 * @return 0 on success, -1 on error
 */
int app_start (app_data_t * app, app_run_in_separate_task_t task_config);

/**
 * Application task definition. Handles events in eternal loop.
 *
 * @param arg                 In: Application handle
 */
_Noreturn void app_loop_forever (void * arg);

/**
 * Get P-Net instance from application
 *
 * @param app                 In:    Application handle
 * @return P-Net instance, NULL on failure
 */
pnet_t * app_get_pnet_instance (app_data_t * app);

#ifdef __cplusplus
}
#endif

#endif /* SAMPLEAPP_COMMON_H */
