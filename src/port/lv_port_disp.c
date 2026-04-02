/**
 * @file lv_port_disp_templ.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "new_thread0.h"
#include "touch_GT911.h"
#include "lv_port_disp.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static void glcdc_flush_finish_event(lv_event_t * event);

static void glcdc_flush_finish_event(lv_event_t * event)
{
    lv_display_t * disp;

    if (LV_EVENT_FLUSH_FINISH == lv_event_get_code(event))
    {
        /* Enable Backlight */
        R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_BLEN, BSP_IO_LEVEL_HIGH);

        disp = lv_event_get_target(event);

        /* now the backlight in enabled, remove the event callback */
        lv_display_remove_event_cb_with_user_data(disp, glcdc_flush_finish_event, NULL);
    }
}

void lvgl_glcdc_callback(rm_lvgl_port_callback_args_t *p_arg)
{
    if (RM_LVGL_PORT_EVENT_UNDERFLOW == p_arg->event)
    {
        assert(0);
    }
}

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/

    /* Shared RESET between the GT911 touch controller and the LCD, only reset once, here */
    disp_init();

    /*------------------------------------
     * Create a display and set a flush_cb
     * -----------------------------------*/
    fsp_err_t err;
    err = RM_LVGL_PORT_Open(&g_lvgl_port_ctrl, &g_lvgl_port_cfg);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    lv_display_add_event_cb(g_lvgl_port_ctrl.p_lv_display, glcdc_flush_finish_event, LV_EVENT_FLUSH_FINISH, NULL);
}

static void disp_init(void)
{
    fsp_err_t err;

    /* Open the I2C bus if it is not already open. */
    rm_comms_i2c_bus_extended_cfg_t * p_extend          = (rm_comms_i2c_bus_extended_cfg_t *) g_comms_i2c_device0_cfg.p_extend;
    i2c_master_instance_t           * p_driver_instance = (i2c_master_instance_t *) p_extend->p_driver_instance;

    err = p_driver_instance->p_api->open(p_driver_instance->p_ctrl, p_driver_instance->p_cfg);
    assert(FSP_SUCCESS == err);

#if (2 == BSP_CFG_RTOS)
    /* Create a semaphore for blocking if a semaphore is not NULL */
    if (NULL != p_extend->p_blocking_semaphore)
    {
              // FreeRTOS
        *(p_extend->p_blocking_semaphore->p_semaphore_handle) =
            xSemaphoreCreateCountingStatic((UBaseType_t) 1,
                                           (UBaseType_t) 0,
                                           p_extend->p_blocking_semaphore->p_semaphore_memory);
    }
    /* Create a recursive mutex for bus lock if a recursive mutex is not NULL */
    if (NULL != p_extend->p_bus_recursive_mutex)
    {
            // FreeRTOS
        *(p_extend->p_bus_recursive_mutex->p_mutex_handle) =
            xSemaphoreCreateRecursiveMutexStatic(p_extend->p_bus_recursive_mutex->p_mutex_memory);
    }
#endif

    /* Need to initialise the Touch Controller before the LCD, as only a Single Reset line shared between them */
    err = RM_COMMS_I2C_Open(&g_comms_i2c_device0_ctrl, &g_comms_i2c_device0_cfg);
    assert(FSP_SUCCESS == err);

    err = init_ts(&g_comms_i2c_device0_ctrl);
    assert(FSP_SUCCESS == err);

    err = enable_ts(&g_comms_i2c_device0_ctrl);
    assert(FSP_SUCCESS == err);
}
